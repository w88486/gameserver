#pragma once
#include<zinx.h>
#include "GameWorld.h"
#include "GameMsg.h"
class GameProtol;
class GameRole :
    public Irole, public Player
{
public:
    GameProtol *m_proto=NULL;
    int ID;
    string username;
    float x;
    float y;//高度
    float z;
    float v;
    GameRole();
    // 通过 Irole 继承
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    GameMsg* CreateIdNameLogin();
    GameMsg* CreateSrdPlayers();
    GameMsg* CreateSelfPosition();
    GameMsg* CreateIdNameLogoff();
    GameMsg* CreateChat(string content);
    void sendChat(GameMsg* data);
    void sendNewPosition(GameMsg* data);
    void viewAppear(GameRole *orole);
    void viewDisAppear(GameRole* orole);
    void Fini() override;

    // 通过 Player 继承
    int GetX() override;
    int GetY() override;
};

