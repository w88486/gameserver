#include "GameWorld.h"

AOIWorld::AOIWorld(int SX, int EX, int SY, int EY, int CX, int CY) :
    StartX(SX), EndX(EX), StartY(SY), EndY(EY), CountXgrid(CX), CountYgrid(CY)
{
    //计算格子宽度
    GridXwidth = (EX - SX) / CX;
    GridYwidth = (EY - SY) / CY;

    //创建格子们
    for (int i = 0; i < CountXgrid * CountYgrid; ++i) {
        Grid tmp;
        world.push_back(tmp);
    }
}
list<Player*> AOIWorld::GetSrdPlayers(Player *player)
{
    list<Player*> ret;
    //获取格子编号
    int id = (player->GetX() - StartX) / GridXwidth + (player->GetY() - StartY) / GridYwidth * CountXgrid;

    //计算格子在网格的第几行第几列
    int x_id = id % CountXgrid;
    int y_id = id / CountYgrid;
    //添加左上角
    if (x_id >= 0 && y_id >= 0) {
        auto tmp = world[id - CountXgrid - 1].Players;
        ret.insert(ret.begin(), tmp.begin(), tmp.end());
    }
    //添加上方
    if (y_id >= 0) {
        auto tmp = world[id - CountXgrid].Players;
        ret.insert(ret.begin(), tmp.begin(), tmp.end());
    }
    //添加右上角
    if (x_id < CountXgrid && y_id >= 0) {
        auto tmp = world[id - CountXgrid + 1].Players;
        ret.insert(ret.begin(), tmp.begin(), tmp.end());
    }
    //添加右方
    if (x_id < CountXgrid) {
        auto tmp = world[id + 1].Players;
        ret.insert(ret.begin(), tmp.begin(), tmp.end());
    }
    //添加右下角
    if (x_id < CountXgrid && y_id < CountYgrid) {
        auto tmp = world[id + CountXgrid + 1].Players;
        ret.insert(ret.begin(), tmp.begin(), tmp.end());
    }
    //添加下方
    if (y_id < CountYgrid) {
        auto tmp = world[id + CountXgrid].Players;
        ret.insert(ret.begin(), tmp.begin(), tmp.end());
    }
    //添加左下角
    if (x_id >= 0 && y_id < CountYgrid) {
        auto tmp = world[id + CountXgrid - 1].Players;
        ret.insert(ret.begin(), tmp.begin(), tmp.end());
    }
    //添加左方
    if (x_id >= 0) {
        auto tmp = world[id - 1].Players;
        ret.insert(ret.begin(), tmp.begin(), tmp.end());
    }
    //添加自己
    ret.push_back(player);
    return ret;
}
bool AOIWorld::AddPlay(Player *player)
{
    //计算格子编号
    int id = (player->GetX() - StartX) / GridXwidth + (player->GetY() - StartY) / GridYwidth * CountXgrid;
    //添加玩家
    world[id].Players.push_back(player);
    return true;
}

bool AOIWorld::DelPlay(Player *player)
{
    //计算格子编号
    int id = (player->GetX() - StartX) / GridXwidth + (player->GetY() - StartY) / GridYwidth * CountXgrid;
    //添加玩家
    world[id].Players.remove(player);
    return false;
}
