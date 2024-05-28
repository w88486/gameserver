#include "Echo.h"
#include"CmdMsg.h"
#include"CmdProcess.h"
bool Echo::Init()
{
    return true;
}

UserData* Echo::ProcMsg(UserData& _poUserData)
{
    //写数据出去
    GET_REF2DATA(CmdMsg, input, _poUserData);
    auto *output = new CmdMsg(input);
    ZinxKernel::Zinx_SendOut(*output,*(CmdProcess::getInstance()));
    return nullptr;
}

void Echo::Fini()
{
}

bool AddDate::Init()
{
    for (auto *role : ZinxKernel::Zinx_GetAllRole()) {
        auto nextst = dynamic_cast<Echo*>(role);
        if (role != nullptr) {
            SetNextProcessor(*nextst);
            break;
        }
    }
    return true;
}

UserData* AddDate::ProcMsg(UserData& _poUserData)
{
    GET_REF2DATA(CmdMsg, userdata, _poUserData);
    auto *newdata=new CmdMsg();
    if (userdata.isCmd == 2) {
        needdata = userdata.needDate;
        return nullptr;
    }
    if (needdata) {
        //获取当前时间
        time_t t = time(0);
        string date = ctime(&t);
        string newstr = date.substr(0, date.length() - 1) + ": " + userdata.szUserData;//添加时间前缀
        //创建新的字符串数据，并返回
        newdata->szUserData = newstr;
    }
    else {
        newdata->szUserData = userdata.szUserData;
    }
    return newdata;
}

void AddDate::Fini()
{
}
