#include<zinx.h>
#include<string>
using namespace std;
#pragma once
class GameProtol :
    public Iprotocol
{
    // Í¨¹ý Iprotocol ¼Ì³Ð
    UserData* raw2request(std::string _szInput) override;
    std::string* response2raw(UserData& _oUserData) override;
    Irole* GetMsgProcessor(UserDataMsg& _oUserDataMsg) override;
    Ichannel* GetMsgSender(BytesMsg& _oBytes) override;
public:
    string m_beforebuf;
    Ichannel *m_channel=NULL;
    Irole* m_role = NULL;
    virtual ~GameProtol();
};

