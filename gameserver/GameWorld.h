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
	//���캯������ʼ����������xy�߽硢��������ÿ�����ӿ��
	AOIWorld(int StartX, int EndX, int StartY, int EndY, int CountXgrid, int CountYgrid);

	//��ȡ��Χ���
	list<Player*> GetSrdPlayers(Player *player);

	//���ɾ�����
	bool AddPlay(Player *player);
	bool DelPlay(Player *player);
};

