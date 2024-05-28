#include "StdInOutChannel.h"
#include "CmdProcess.h"
#include<iostream>
using namespace std;
//从标准输入输入
bool StdInChannel::Init()
{
	return true;
}

bool StdInChannel::ReadFd(std::string& _input)
{
	cin >> _input;
	return true;
}

bool StdInChannel::WriteFd(std::string& _output)
{
	return false;
}

void StdInChannel::Fini()
{
}

int StdInChannel::GetFd()
{
	return 0;
}

std::string StdInChannel::GetChannelInfo()
{
	return std::string("stdin");
}

AZinxHandler* StdInChannel::GetInputNextStage(BytesMsg& _oInput)
{
	//下一步交给自定义的cmd协议对象处理
	return CmdProcess::getInstance();
}


//输出到标准输出
bool StdOutChannel::Init()
{
	return true;
}

bool StdOutChannel::ReadFd(std::string& _input)
{
	return false;
}

bool StdOutChannel::WriteFd(std::string& _output)
{
	cout << _output << endl;
	return true;
}

void StdOutChannel::Fini()
{
}

int StdOutChannel::GetFd()
{
	return 1;
}

std::string StdOutChannel::GetChannelInfo()
{
	return std::string("stdout");
}

AZinxHandler* StdOutChannel::GetInputNextStage(BytesMsg& _oInput)
{
	return nullptr;
}

AZinxHandler* MyTcp::GetInputNextStage(BytesMsg& _oInput)
{
	//下一步交给自定义的cmd协议对象处理
	return CmdProcess::getInstance();
}

ZinxTcpData* TcpConnectFact::CreateTcpDataChannel(int _fd)
{
	return new MyTcp(_fd);
}
