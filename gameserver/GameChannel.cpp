#include "GameChannel.h"
#include "GameRole.h"
#include "GameProtol.h"
GameChannel::~GameChannel()
{
	if (NULL != m_proto) {
		delete m_proto;
	}
}
AZinxHandler* GameChannel::GetInputNextStage(BytesMsg& _oInput)
{
	return m_proto;
}

ZinxTcpData* GameTcpChannelFact::CreateTcpDataChannel(int _fd)
{
	//创建tcp协议、角色和通道对象
	auto channel = new GameChannel(_fd);
	auto protol = new GameProtol();
	auto role = new GameRole();

	//通道和协议绑定
	protol->m_channel = channel;
	channel->m_proto = protol;

	//角色和协议绑定
	protol->m_role = role;
	role->m_proto = protol;

	//将协议和角色对象添加进内核
	ZinxKernel::Zinx_Add_Role(*role);
	ZinxKernel::Zinx_Add_Proto(*protol);
	return channel;
}
