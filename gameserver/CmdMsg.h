#pragma once
#include <zinx.h>
#include<string>
using namespace std;
class CmdMsg :
    public UserData
{
public:
    string szUserData;
    short isCmd=0;
    bool isOpen=true;
    bool needDate=false;
    CmdMsg() {};
};

