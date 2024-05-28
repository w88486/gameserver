#pragma once
#include <zinx.h>
#include<string>
#include<google/protobuf/message.h>
#include<list>
using namespace std;
class GameMsg :
    public UserData
{
public:
    google::protobuf::Message* Msg = NULL;
    enum MSG_TYPE {
        MSG_LOGIN_ID_NAME = 1,
        MSG_CHAT_CONTENT = 2,
        MSG_NEW_POSITION = 3,
        MSG_BROADCAST = 200,
        MSG_LOGINOUT_ID_NAME = 201,
        MSG_OTHER_PLAY_POSITION = 202
    }Type;
    GameMsg(MSG_TYPE _type, google::protobuf::Message *_msg);
    GameMsg(MSG_TYPE _type, string _stream);
    string serialize();
    virtual ~GameMsg();
};
class MutiMsg :
    public UserData
{
public:
    list<GameMsg*>MsgList;
};

