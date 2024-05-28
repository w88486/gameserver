#include "GameRole.h"
#include "GameMsg.h"
#include "msg.pb.h"
#include "GameChannel.h"
#include "GameProtol.h"
#include<random>
#include<time.h>
#include "ZInxTimer.h"
//�����������
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
    //����ǵ�һ����ң�ժ����ʱ��
    if (ZinxKernel::Zinx_GetAllRole().size() <= 0) {
        ZInxTimerMng::GetInstance()->RemoveTimeHandler(&e_task);
    }
    bool pret = GameWorld.AddPlay(this);
    //����ID,username
    ID = m_proto->m_channel->GetFd();
    username = "Tom";
    if (true == pret) {
        //���Լ�����id��name
        auto msg = CreateIdNameLogin();
        ZinxKernel::Zinx_SendOut(*msg, *m_proto);
        //���Լ�������Χ���λ��
        msg = CreateSrdPlayers();
        ZinxKernel::Zinx_SendOut(*msg, *m_proto);
        //����Χ��ҷ����Լ�λ��
        auto plist = GameWorld.GetSrdPlayers(this);
        for (auto single : plist) {
            msg = CreateSelfPosition();
            auto role = dynamic_cast<GameRole*>(single);
            ZinxKernel::Zinx_SendOut(*msg, *(role->m_proto));
        }
    }
    return pret;
}
//��Ұ���֣����෢��200����
void GameRole::viewAppear(GameRole* orole)
{
    //���Լ���������λ��
    auto msg = orole->CreateSelfPosition();
    ZinxKernel::Zinx_SendOut(*msg, *m_proto);
    //��������Լ���λ��
    msg = CreateSelfPosition();
    ZinxKernel::Zinx_SendOut(*msg, *(orole->m_proto));
}
//��Ұ��ʧ�����෢��201����
void GameRole::viewDisAppear(GameRole* orole)
{
    //���Լ���������λ��
    auto msg = orole->CreateIdNameLogoff();
    ZinxKernel::Zinx_SendOut(*msg, *m_proto);
    //��������Լ���λ��
    msg = CreateIdNameLogoff();
    ZinxKernel::Zinx_SendOut(*msg, *(orole->m_proto));

}
void GameRole::sendChat(GameMsg *data) {
    //ȡ����������
    auto content = dynamic_cast<pb::Talk*>(data->Msg)->content();
    //��������ҷ���������Ϣ
    auto role_list = ZinxKernel::Zinx_GetAllRole();
    for (auto role : role_list) {
        auto gamerole = dynamic_cast<GameRole*>(role);
        auto msg = CreateChat(content);
        ZinxKernel::Zinx_SendOut(*msg, *(gamerole->m_proto));
    }
}
void GameRole::sendNewPosition(GameMsg* data) {
    //1��ȡ��λ��
    //2�㲥��λ�ø���Χ���
    //3��������

    //ȡ����λ��
    auto newPos = dynamic_cast<pb::Position*>(data->Msg);
    //��ȡԭ�����ھ�s1
    auto s1 = GameWorld.GetSrdPlayers(this);
    // ���¸�����ժ��
    GameWorld.DelPlay(this);
    // ����λ��
    x = newPos->x();
    y = newPos->y();
    z = newPos->z();
    v = newPos->v();
    // �����¸���
    GameWorld.AddPlay(this);
    //��ȡ���ھ�s2
    auto s2 = GameWorld.GetSrdPlayers(this);
    //����s2,������s1����Ұ���֣����෢��200����
    for (auto single : s2) {
        if (s1.end() == find(s1.begin(), s1.end(), single)){
            viewAppear(dynamic_cast<GameRole*>(single));
        }
    }
    //����s1,������s2����Ұ��ʧ�����෢��201����
    for (auto single : s1) {
        if (s2.end() == find(s2.begin(), s2.end(), single)) {
            viewDisAppear(dynamic_cast<GameRole*>(single));
        }
    }
    //������Χ��Ҳ������Լ�λ��
    auto plist = GameWorld.GetSrdPlayers(this);
    for (auto single : plist) {
        //��ɱ���
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
//����1����Ϣ�����������Ϣ��Ϣ
GameMsg* GameRole::CreateIdNameLogin()
{
    auto pmsg = new pb::SyncPid();
    pmsg->set_pid(ID);
    pmsg->set_username(username);
    auto ret = new GameMsg(GameMsg::MSG_LOGIN_ID_NAME,pmsg);
    return ret;
}
//����202�����������Ϣ
GameMsg* GameRole::CreateSrdPlayers()
{
    auto pmsg = new pb::SyncPlayer();
    //ͨ��addXXX��mutableXXX���һ�������Ա������Ϣ�����������Աָ�������Ϣָ��
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
//����200�Ź㲥��Ϣ
GameMsg* GameRole::CreateSelfPosition()
{
    auto pmsg = new pb::BroadCast();
    //ͨ��mutableXXX�������Ϣ����������Ϣָ��
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
//����201��������Ϣ
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
    //ͨ��mutableXXX�������Ϣ����������Ϣָ��
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

    //��������һ����ң������ö�ʱ���˳�����
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
