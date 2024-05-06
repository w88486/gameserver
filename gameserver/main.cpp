#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <zinx.h>
using namespace std;
class TestStdout :public Ichannel {
	// 通过 Ichannel 继承
	bool Init() override
	{
		return true;
	}
	bool ReadFd(std::string& _input) override
	{
		return false;
	}
	bool WriteFd(std::string& _output) override
	{
		cout << _output << endl;
		return true;
	}
	void Fini() override
	{
	}
	int GetFd() override
	{
		return 1;
	}
	std::string GetChannelInfo() override
	{
		return std::string("stdout");
	}
	AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override
	{
		return nullptr;
	}
}*poStdout = new TestStdout();
class Echo :public AZinxHandler {
	// 2通过 AZinxHandler 继承，重写虚函数，进行处理
	IZinxMsg* InternelHandle(IZinxMsg& _oInput) override
	{
		//2回显到标准输出
		GET_REF2DATA(BytesMsg, input, _oInput);
		Ichannel* poOut = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
		if (poOut != nullptr) {
			ZinxKernel::Zinx_SendOut(input.szData, *poOut);
		}
		cout << input.szData << endl;
		return nullptr;
	}
	AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override
	{

		return nullptr;
	}
}*poEcho=new Echo();
class TestStdin :public Ichannel {
	// 3通过 Ichannel 继承，完成数据收发，GetinputNextStage返回第2步中的对象
	bool Init() override
	{
		//如果要启用该channel一定要返回true
		return true;
	}
	bool ReadFd(std::string& _input) override
	{
		//输入
		cin >> _input;
		return true;
	}
	bool WriteFd(std::string& _output) override
	{
		return false;
	}
	void Fini() override
	{
	}
	int GetFd() override
	{
		return 0;
	}
	std::string GetChannelInfo() override
	{
		return std::string("stdin");
	}
	AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override
	{
		return poEcho;
	}
};
int main() {
	//1初始化框架
	ZinxKernel::ZinxKernelInit();

	//4将通道对象添加到框架
	TestStdin* poStdin = new TestStdin();
	ZinxKernel::Zinx_Add_Channel(*poStdin);
//	ZinxKernel::Zinx_Add_Channel(*poStdout);

	//5运行框架
	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return 0;
}