#pragma once
#include <zinx.h>
class Echo :
    public Irole
{
    // ͨ�� Irole �̳�
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void Fini() override;
};
class AddDate :
    public Irole 
{
    // ͨ�� Irole �̳�
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void Fini() override;
public:
    bool needdata = false;
};

