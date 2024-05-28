#include"ZInxTimer.h"
#include<sys/timerfd.h>
#include<unistd.h>
#include<string>
IZinxMsg* ZInxTimerMng::InternelHandle(IZinxMsg& _oInput)
{
	GET_REF2DATA(BytesMsg, input, _oInput);
	unsigned long timeout_count = 0;
	input.szData.copy((char*)&timeout_count, sizeof(timeout_count), 0);
	while (timeout_count-- > 0) { 
		list<ZinxTimerHandler*>h_cache;//����Ļ���
		//�ƶ��̶�
		++cur_index;
		cur_index = cur_index % size_;
		for (auto handler = TimelistCircle[cur_index].begin(); handler != TimelistCircle[cur_index].end();) {
			--((*handler)->icount);//Ȧ����һ
			if ((*handler)->icount <= 0) {//���Ȧ�����㣬�򻺴�����
				h_cache.push_back(*handler);
				auto pitr = *handler;
				handler = TimelistCircle[cur_index].erase(handler);//ժ��������
				AddTimeHandler(pitr);
			}
			else {//����Ͳ�����һ��
				++handler;
			}
		}
		//ִ�л����е����񣬲���������һ��Ӧ��ִ�и�����ĳ���Ӹ�����
		for (auto handler : h_cache) {
			handler->proc();
		}
	}
	return nullptr;
}

AZinxHandler* ZInxTimerMng::GetNextHandler(IZinxMsg& _oNextMsg)
{
	return nullptr;
}
void ZInxTimerMng::AddTimeHandler(ZinxTimerHandler* handler)
{
	handler->icount = (cur_index + handler->GetTimerSec()) / size_;
	//����Ӧ��ӵ��ĳݵ�λ��
	int index = (cur_index+handler->GetTimerSec())%size_;
	//��������
	TimelistCircle[index].push_back(handler);
}
void ZInxTimerMng::RemoveTimeHandler(ZinxTimerHandler* handler)
{
	//���������б�ɾ��handler
	for (auto& list : TimelistCircle) {
		for (auto lhandler : list) {
			if (lhandler == handler) {
				list.remove(lhandler);
				break;
			}
		}
	}
}
bool ZInxTimerChannel::Init()
{
	//�����ļ�������
	bool isopen = false;
	int fd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (fd >= 0) {
		//����ʱ������
		struct itimerspec inter = {
			{3,0},{3,0}
		};
		if (0 == timerfd_settime(fd, 0, &inter, nullptr)) {
			isopen = true;
			m_timefd = fd;
		}
	}
	return isopen;
}

bool ZInxTimerChannel::ReadFd(std::string& _input)
{
	bool bret = false;
	char ret[8] = { 0 };
	if (sizeof(ret) == read(m_timefd, ret, sizeof(ret))){
		bret = true;
		_input.assign(ret,sizeof(ret));
	}
	return bret;
}

bool ZInxTimerChannel::WriteFd(std::string& _output)
{
	return false;
}

void ZInxTimerChannel::Fini()
{
	close(m_timefd);
	m_timefd = -1;
}

int ZInxTimerChannel::GetFd()
{
	return m_timefd;
}

std::string ZInxTimerChannel::GetChannelInfo()
{
	return std::string("timefd");
}
ZInxTimerMng* ZInxTimerMng::single =new ZInxTimerMng() ;
AZinxHandler* ZInxTimerChannel::GetInputNextStage(BytesMsg& _oInput)
{
	return ZInxTimerMng::GetInstance();
}
/*
void out_hello::proc()
{
	auto channel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
	std::string output = "hello world";
	ZinxKernel::Zinx_SendOut(output, *channel);
	ZInxTimerMng::GetInstance()->RemoveTimeHandler(this);
}

int out_hello::GetTimerSec()
{
	return 5;
}

void out_bye::proc()
{
	auto channel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
	std::string output = "bye";
	ZinxKernel::Zinx_SendOut(output, *channel);
}

int out_bye::GetTimerSec()
{
	return 3;
}
*/
