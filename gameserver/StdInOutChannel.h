#pragma once
#include <zinx.h>
#include<ZinxTCP.h>
class StdInChannel :
    public Ichannel
{
    // 通过 Ichannel 继承
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
    // 通过 Ichannel 继承
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
    // 通过 ZinxTcpData 继承
    AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
public:
    MyTcp(int _fd) :ZinxTcpData(_fd) {};
};

class TcpConnectFact :
    public IZinxTcpConnFact 
{
    // 通过 IZinxTcpConnFact 继承
    ZinxTcpData* CreateTcpDataChannel(int _fd) override;
};
