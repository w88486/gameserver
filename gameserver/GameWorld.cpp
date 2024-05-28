#include "GameWorld.h"

AOIWorld::AOIWorld(int SX, int EX, int SY, int EY, int CX, int CY) :
    StartX(SX), EndX(EX), StartY(SY), EndY(EY), CountXgrid(CX), CountYgrid(CY)
{
    //������ӿ��
    GridXwidth = (EX - SX) / CX;
    GridYwidth = (EY - SY) / CY;

    //����������
    for (int i = 0; i < CountXgrid * CountYgrid; ++i) {
        Grid tmp;
        world.push_back(tmp);
    }
}
list<Player*> AOIWorld::GetSrdPlayers(Player *player)
{
    list<Player*> ret;
    //��ȡ���ӱ��
    int id = (player->GetX() - StartX) / GridXwidth + (player->GetY() - StartY) / GridYwidth * CountXgrid;

    //�������������ĵڼ��еڼ���
    int x_id = id % CountXgrid;
    int y_id = id / CountYgrid;
    //������Ͻ�
    if (x_id >= 0 && y_id >= 0) {
        auto tmp = world[id - CountXgrid - 1].Players;
        ret.insert(ret.begin(), tmp.begin(), tmp.end());
    }
    //����Ϸ�
    if (y_id >= 0) {
        auto tmp = world[id - CountXgrid].Players;
        ret.insert(ret.begin(), tmp.begin(), tmp.end());
    }
    //������Ͻ�
    if (x_id < CountXgrid && y_id >= 0) {
        auto tmp = world[id - CountXgrid + 1].Players;
        ret.insert(ret.begin(), tmp.begin(), tmp.end());
    }
    //����ҷ�
    if (x_id < CountXgrid) {
        auto tmp = world[id + 1].Players;
        ret.insert(ret.begin(), tmp.begin(), tmp.end());
    }
    //������½�
    if (x_id < CountXgrid && y_id < CountYgrid) {
        auto tmp = world[id + CountXgrid + 1].Players;
        ret.insert(ret.begin(), tmp.begin(), tmp.end());
    }
    //����·�
    if (y_id < CountYgrid) {
        auto tmp = world[id + CountXgrid].Players;
        ret.insert(ret.begin(), tmp.begin(), tmp.end());
    }
    //������½�
    if (x_id >= 0 && y_id < CountYgrid) {
        auto tmp = world[id + CountXgrid - 1].Players;
        ret.insert(ret.begin(), tmp.begin(), tmp.end());
    }
    //�����
    if (x_id >= 0) {
        auto tmp = world[id - 1].Players;
        ret.insert(ret.begin(), tmp.begin(), tmp.end());
    }
    //����Լ�
    ret.push_back(player);
    return ret;
}
bool AOIWorld::AddPlay(Player *player)
{
    //������ӱ��
    int id = (player->GetX() - StartX) / GridXwidth + (player->GetY() - StartY) / GridYwidth * CountXgrid;
    //������
    world[id].Players.push_back(player);
    return true;
}

bool AOIWorld::DelPlay(Player *player)
{
    //������ӱ��
    int id = (player->GetX() - StartX) / GridXwidth + (player->GetY() - StartY) / GridYwidth * CountXgrid;
    //������
    world[id].Players.remove(player);
    return false;
}
