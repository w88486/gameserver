#pragma once

#include <zinx.h>
#include <string>
using namespace std;
class CmdProcess :
    public Iprotocol
{
    static CmdProcess* single;
    CmdProcess() {};
    
public:
    static CmdProcess* getInstance() {
        return single;
    }
    bool needDate = false;
    bool isOpen = true;
    string OutChannel;
    // Í¨¹ý Iprotocol ¼Ì³Ð
    UserData* raw2request(std::string _szInput) override;
    std::string* response2raw(UserData& _oUserData) override;
    Irole* GetMsgProcessor(UserDataMsg& _oUserDataMsg) override;
    Ichannel* GetMsgSender(BytesMsg& _oBytes) override;
};

