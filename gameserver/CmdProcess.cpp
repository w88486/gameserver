#include "CmdProcess.h"
#include"StdInOutChannel.h"
#include"CmdMsg.h"
#include"Echo.h"
#include"OpenCtl.h"

CmdProcess* CmdProcess::single = new CmdProcess();

UserData* CmdProcess::raw2request(std::string _szInput)
{
    auto userdata = new CmdMsg();
    if (_szInput == "open") {
        userdata->isCmd = 1;
        userdata->isOpen = true;
    }else if (_szInput == "close") {
        userdata->isCmd = 1;
        userdata->isOpen = false;
    }else if (_szInput == "date") {
        userdata->isCmd = 2;
        userdata->needDate = true;
    }
    else if (_szInput == "cleardate") {
        userdata->isCmd = 2;
        userdata->needDate = false;
    }
    else if (_szInput == "exit") {
        ZinxKernel::Zinx_Exit();
        return nullptr;
    }
    userdata->szUserData = _szInput;
    return userdata;
}

std::string* CmdProcess::response2raw(UserData& _oUserData)
{
    GET_REF2DATA(CmdMsg, output, _oUserData);
    return new string(output.szUserData);

}

Irole* CmdProcess::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{
    OutChannel = _oUserDataMsg.szInfo;
    if (OutChannel == "stdin") {
        OutChannel = "stdout";
    }
    GET_REF2DATA(CmdMsg, userdata, *_oUserDataMsg.poUserData);
    //选择业务层，并返回
    Irole* retRole = nullptr;
    auto rolelist=ZinxKernel::Zinx_GetAllRole();
    if (userdata.isCmd==1) {
        for (auto* role : rolelist) {
            auto ctl = dynamic_cast<OpenCtl*>(role);
            if (ctl != nullptr) {
                retRole = ctl;
                break;
            }
            
        }
    }
    else{
        for (auto* role : rolelist) {
            auto dateadd = dynamic_cast<AddDate*>(role);
            if (dateadd != nullptr) {
                retRole = dateadd;
                break;
            }
        }
    }
    return retRole;
}

Ichannel* CmdProcess::GetMsgSender(BytesMsg& _oBytes)
{
    //根据消息管道信息返回响应管道
    return ZinxKernel::Zinx_GetChannel_ByInfo(OutChannel);
}
