#include "GameMsg.h"
#include"msg.pb.h"
GameMsg::GameMsg(MSG_TYPE _type, google::protobuf::Message* _msg):Type(_type),Msg(_msg)
{

}

GameMsg::GameMsg(MSG_TYPE _type, string _stream):Type(_type)
{
    switch (_type) {
        case MSG_LOGIN_ID_NAME :
            Msg = new pb::SyncPid();
            break;
        case MSG_CHAT_CONTENT:
            Msg = new pb::Talk();
            break;
        case MSG_NEW_POSITION:
            Msg = new pb::Position();
            break;
        case MSG_BROADCAST:
            Msg = new pb::BroadCast();
            break;
        case MSG_LOGINOUT_ID_NAME:
            Msg = new pb::SyncPid();
            break;
        case MSG_OTHER_PLAY_POSITION:
            Msg = new pb::SyncPlayer();
            break;
    }
    Msg->ParseFromString(_stream);
}

string GameMsg::serialize()
{
    string ret;
    Msg->SerializeToString(&ret);
    return ret;
}

GameMsg::~GameMsg()
{
    if (NULL != Msg) {
        delete Msg;
    }
}
