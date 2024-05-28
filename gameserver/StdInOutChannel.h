#pragma once
#include <zinx.h>
#include<ZinxTCP.h>
class StdInChannel :
    public Ichannel
{
    // ͨ�� Ichannel �̳�
    bool Init() override;
    bool ReadFd(std::string& _input) override;
    bool WriteFd(std::string& _output) override;
    void Fini() override;
    int GetFd() override;
    std::string GetChannelInfo() override;
    AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};
class StdOutChannel :
    public Ichannel
{
    // ͨ�� Ichannel �̳�
    bool Init() override;
    bool ReadFd(std::string& _input) override;
    bool WriteFd(std::string& _output) override;
    void Fini() override;
    int GetFd() override;
    std::string GetChannelInfo() override;
    AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};

class MyTcp :
    public ZinxTcpData 
{
    // ͨ�� ZinxTcpData �̳�
    AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
public:
    MyTcp(int _fd) :ZinxTcpData(_fd) {};
};

class TcpConnectFact :
    public IZinxTcpConnFact 
{
    // ͨ�� IZinxTcpConnFact �̳�
    ZinxTcpData* CreateTcpDataChannel(int _fd) override;
};
