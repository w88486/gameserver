#pragma once
#include<zinx.h>
#include<ZinxTCP.h>
class GameProtol;
class GameChannel :
    public ZinxTcpData
{
public:
    GameProtol *m_proto=NULL;
    GameChannel(int _fd) :ZinxTcpData(_fd) {};
    virtual ~GameChannel();
    // 通过 ZinxTcpData 继承
    AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};
class GameTcpChannelFact :
    public IZinxTcpConnFact
{
    // 通过 IZinxTcpConnFact 继承
    ZinxTcpData* CreateTcpDataChannel(int _fd) override;
};

