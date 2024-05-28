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
    // ͨ�� ZinxTcpData �̳�
    AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};
class GameTcpChannelFact :
    public IZinxTcpConnFact
{
    // ͨ�� IZinxTcpConnFact �̳�
    ZinxTcpData* CreateTcpDataChannel(int _fd) override;
};

