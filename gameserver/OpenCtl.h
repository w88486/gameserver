#pragma once
#include <zinx.h>
class OpenCtl :
    public Irole
{
    // ͨ�� Irole �̳�
    Ichannel *prechannel=nullptr;
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void Fini() override;
};

