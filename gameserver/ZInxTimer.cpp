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
		list<ZinxTimerHandler*>h_cache;//任务的缓存
		//移动刻度
		++cur_index;
		cur_index = cur_index % size_;
		for (auto handler = TimelistCircle[cur_index].begin(); handler != TimelistCircle[cur_index].end();) {
			--((*handler)->icount);//圈数减一
			if ((*handler)->icount <= 0) {//如果圈数清零，则缓存任务
				h_cache.push_back(*handler);
				auto pitr = *handler;
				handler = TimelistCircle[cur_index].erase(handler);//摘除该任务
				AddTimeHandler(pitr);
			}
			else {//否则就查找下一个
				++handler;
			}
		}
		//执行缓存中的任务，并重新在下一个应该执行该任务的齿添加该任务
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
	//计算应添加到的齿的位置
	int index = (cur_index+handler->GetTimerSec())%size_;
	//插入任务
	TimelistCircle[index].push_back(handler);
}
void ZInxTimerMng::RemoveTimeHandler(ZinxTimerHandler* handler)
{
	//遍历所有列表，删除handler
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
	//创建文件描述符
	bool isopen = false;
	int fd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (fd >= 0) {
		//设置时间周期
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
