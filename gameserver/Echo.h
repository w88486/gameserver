#pragma once
#include <zinx.h>
class Echo :
    public Irole
{
    // 通过 Irole 继承
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void Fini() override;
};
class AddDate :
    public Irole 
{
    // 通过 Irole 继承
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void Fini() override;
public:
    bool needdata = false;
};

