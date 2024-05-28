#pragma once
#include<list>
#include<vector>
using namespace std;
class Player {
public:
	virtual int GetX() = 0;
	virtual int GetY() = 0;
};

class Grid {
public:
	list<Player*>Players;
};

class AOIWorld
{
	vector<Grid>world;
	int StartX;
	int EndX;
	int StartY;
	int EndY;
	int CountXgrid;
	int CountYgrid;
	int GridXwidth;
	int GridYwidth;
public:
	//构造函数，初始化世界对象的xy边界、格子数和每个格子宽度
	AOIWorld(int StartX, int EndX, int StartY, int EndY, int CountXgrid, int CountYgrid);

	//获取周围玩家
	list<Player*> GetSrdPlayers(Player *player);

	//添加删除玩家
	bool AddPlay(Player *player);
	bool DelPlay(Player *player);
};

