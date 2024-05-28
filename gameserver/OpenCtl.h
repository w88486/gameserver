#pragma once
#include <zinx.h>
class OpenCtl :
    public Irole
{
    // Í¨¹ý Irole ¼Ì³Ð
    Ichannel *prechannel=nullptr;
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void Fini() override;
};

