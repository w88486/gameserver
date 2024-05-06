#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <zinx.h>
using namespace std;
class TestStdout :public Ichannel {
	// ͨ�� Ichannel �̳�
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
	// 2ͨ�� AZinxHandler �̳У���д�麯�������д���
	IZinxMsg* InternelHandle(IZinxMsg& _oInput) override
	{
		//2���Ե���׼���
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
	// 3ͨ�� Ichannel �̳У���������շ���GetinputNextStage���ص�2���еĶ���
	bool Init() override
	{
		//���Ҫ���ø�channelһ��Ҫ����true
		return true;
	}
	bool ReadFd(std::string& _input) override
	{
		//����
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
	//1��ʼ�����
	ZinxKernel::ZinxKernelInit();

	//4��ͨ��������ӵ����
	TestStdin* poStdin = new TestStdin();
	ZinxKernel::Zinx_Add_Channel(*poStdin);
//	ZinxKernel::Zinx_Add_Channel(*poStdout);

	//5���п��
	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return 0;
}