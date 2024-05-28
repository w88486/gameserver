#include "OpenCtl.h"
#include"CmdMsg.h"
bool OpenCtl::Init()
{
    return true;
}

UserData* OpenCtl::ProcMsg(UserData& _poUserData)
{
    GET_REF2DATA(CmdMsg, userdata, _poUserData);
    if (userdata.isOpen) {
        if (prechannel != nullptr) {
            ZinxKernel::Zinx_Add_Channel(*prechannel);
        }
    }
    else {
        prechannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
        ZinxKernel::Zinx_Del_Channel(*prechannel);
    }
    return nullptr;
}

void OpenCtl::Fini()
{
}
