#include "GameRole.h"
#include "GameMsg.h"
#include "msg.pb.h"
#include "GameChannel.h"
#include "GameProtol.h"
#include<random>
#include<time.h>
#include "ZInxTimer.h"
//创建世界对象
static AOIWorld GameWorld(0, 400, 0, 400, 20, 20);
static default_random_engine random_engine(time(0));
class exit_task :public ZinxTimerHandler {
public:
    virtual void proc() {
        ZinxKernel::Zinx_Exit();
    };
    virtual int GetTimerSec() {
        return 20;
    }
};
static exit_task e_task;
GameRole::GameRole()
{
    x = 100 + random_engine() % 50;
    z = 100 + random_engine() % 50;
}

bool GameRole::Init()
{
    //如果是第一个玩家，摘除定时器
    if (ZinxKernel::Zinx_GetAllRole().size() <= 0) {
        ZInxTimerMng::GetInstance()->RemoveTimeHandler(&e_task);
    }
    bool pret = GameWorld.AddPlay(this);
    //设置ID,username
    ID = m_proto->m_channel->GetFd();
    username = "Tom";
    if (true == pret) {
        //向自己发送id、name
        auto msg = CreateIdNameLogin();
        ZinxKernel::Zinx_SendOut(*msg, *m_proto);
        //向自己发送周围玩家位置
        msg = CreateSrdPlayers();
        ZinxKernel::Zinx_SendOut(*msg, *m_proto);
        //向周围玩家发送自己位置
        auto plist = GameWorld.GetSrdPlayers(this);
        for (auto single : plist) {
            msg = CreateSelfPosition();
            auto role = dynamic_cast<GameRole*>(single);
            ZinxKernel::Zinx_SendOut(*msg, *(role->m_proto));
        }
    }
    return pret;
}
//视野出现，互相发送200报文
void GameRole::viewAppear(GameRole* orole)
{
    //向自己发参数的位置
    auto msg = orole->CreateSelfPosition();
    ZinxKernel::Zinx_SendOut(*msg, *m_proto);
    //向参数发自己的位置
    msg = CreateSelfPosition();
    ZinxKernel::Zinx_SendOut(*msg, *(orole->m_proto));
}
//视野消失，互相发送201报文
void GameRole::viewDisAppear(GameRole* orole)
{
    //向自己发参数的位置
    auto msg = orole->CreateIdNameLogoff();
    ZinxKernel::Zinx_SendOut(*msg, *m_proto);
    //向参数发自己的位置
    msg = CreateIdNameLogoff();
    ZinxKernel::Zinx_SendOut(*msg, *(orole->m_proto));

}
void GameRole::sendChat(GameMsg *data) {
    //取出聊天内容
    auto content = dynamic_cast<pb::Talk*>(data->Msg)->content();
    //向所有玩家发送聊天消息
    auto role_list = ZinxKernel::Zinx_GetAllRole();
    for (auto role : role_list) {
        auto gamerole = dynamic_cast<GameRole*>(role);
        auto msg = CreateChat(content);
        ZinxKernel::Zinx_SendOut(*msg, *(gamerole->m_proto));
    }
}
void GameRole::sendNewPosition(GameMsg* data) {
    //1获取新位置
    //2广播新位置给周围玩家
    //3跨网格处理

    //取出新位置
    auto newPos = dynamic_cast<pb::Position*>(data->Msg);
    //获取原来的邻居s1
    auto s1 = GameWorld.GetSrdPlayers(this);
    // 从新格子中摘除
    GameWorld.DelPlay(this);
    // 更新位置
    x = newPos->x();
    y = newPos->y();
    z = newPos->z();
    v = newPos->v();
    // 放入新格子
    GameWorld.AddPlay(this);
    //获取新邻居s2
    auto s2 = GameWorld.GetSrdPlayers(this);
    //遍历s2,不属于s1的视野出现，互相发送200报文
    for (auto single : s2) {
        if (s1.end() == find(s1.begin(), s1.end(), single)){
            viewAppear(dynamic_cast<GameRole*>(single));
        }
    }
    //遍历s1,不属于s2的视野消失，互相发送201报文
    for (auto single : s1) {
        if (s2.end() == find(s2.begin(), s2.end(), single)) {
            viewDisAppear(dynamic_cast<GameRole*>(single));
        }
    }
    //遍历周围玩家并发送自己位置
    auto plist = GameWorld.GetSrdPlayers(this);
    for (auto single : plist) {
        //组成报文
        auto pmsg = new pb::BroadCast();
        pmsg->set_tp(4);
        auto subp = pmsg->mutable_p();
        subp->set_v(newPos->v());
        subp->set_x(newPos->x());
        subp->set_y(newPos->y());
        subp->set_z(newPos->z());
        pmsg->set_pid(ID);
        pmsg->set_username(username);
        auto role = dynamic_cast<GameRole*>(single); 
        ZinxKernel::Zinx_SendOut(*(new GameMsg(GameMsg::MSG_BROADCAST, pmsg)), *(role->m_proto));
    }
}
UserData* GameRole::ProcMsg(UserData& _poUserData)
{
    GET_REF2DATA(MutiMsg, DataList, _poUserData);
    for (auto data : DataList.MsgList) {
        cout <<"type is " << data->Type << endl;
        cout << data->Msg->Utf8DebugString()<<endl;
        switch (data->Type) {
            case GameMsg::MSG_CHAT_CONTENT: 
                sendChat(data);
                break;
            case GameMsg::MSG_NEW_POSITION:
                sendNewPosition(data);
                break;
            default:
                break;
        }
    }
    return nullptr;
}
//返回1号消息，上线玩家信息消息
GameMsg* GameRole::CreateIdNameLogin()
{
    auto pmsg = new pb::SyncPid();
    pmsg->set_pid(ID);
    pmsg->set_username(username);
    auto ret = new GameMsg(GameMsg::MSG_LOGIN_ID_NAME,pmsg);
    return ret;
}
//返回202号其它玩家消息
GameMsg* GameRole::CreateSrdPlayers()
{
    auto pmsg = new pb::SyncPlayer();
    //通过addXXX、mutableXXX添加一个数组成员和子消息，返回数组成员指针和子消息指针
    auto plist = GameWorld.GetSrdPlayers(this);
    for (auto single : plist) {
        auto role = dynamic_cast<GameRole*>(single);
        auto subp = pmsg->add_ps();
        subp->set_pid(role->ID);
        subp->set_username(role->username);
        auto pposition = subp->mutable_p();
        pposition->set_v(role->v);
        pposition->set_x(role->x);
        pposition->set_y(role->y);
        pposition->set_z(role->z);
    }
    auto ret = new GameMsg(GameMsg::MSG_OTHER_PLAY_POSITION, pmsg);
    return ret;
}
//返回200号广播消息
GameMsg* GameRole::CreateSelfPosition()
{
    auto pmsg = new pb::BroadCast();
    //通过mutableXXX添加子消息，返回子消息指针
    pmsg->set_pid(ID);
    pmsg->set_username(username);
    pmsg->set_tp(2);
    auto submsg=pmsg->mutable_p();
    submsg->set_x(x);
    submsg->set_y(y);
    submsg->set_z(z);
    submsg->set_v(v);
    auto ret = new GameMsg(GameMsg::MSG_BROADCAST, pmsg);
    return ret;
}
//返回201，下线消息
GameMsg* GameRole::CreateIdNameLogoff()
{
    auto pmsg = new pb::SyncPid();
    pmsg->set_pid(ID);
    pmsg->set_username(username);
    auto ret = new GameMsg(GameMsg::MSG_LOGINOUT_ID_NAME, pmsg);
    return ret;
}

GameMsg* GameRole::CreateChat(string content)
{
    auto pmsg = new pb::BroadCast();
    //通过mutableXXX添加子消息，返回子消息指针
    pmsg->set_pid(ID);
    pmsg->set_username(username);
    pmsg->set_tp(1);
    pmsg->set_content(content);
    auto ret = new GameMsg(GameMsg::MSG_BROADCAST, pmsg);
    return ret;
    return nullptr;
}



void GameRole::Fini()
{
    auto plist = GameWorld.GetSrdPlayers(this);
    for (auto single : plist) {
        auto msg = CreateIdNameLogoff();
        auto role = dynamic_cast<GameRole*>(single);
        ZinxKernel::Zinx_SendOut(*msg, *(role->m_proto));
    }
    GameWorld.DelPlay(this);

    //如果是最后一个玩家，则设置定时器退出程序
    if (ZinxKernel::Zinx_GetAllRole().size() <= 1) {
        ZInxTimerMng::GetInstance()->AddTimeHandler(&e_task);
    }
}

int GameRole::GetX()
{
    return (int)x;
}

int GameRole::GetY()
{
    return (int)z;
}
