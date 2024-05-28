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
	//����tcpЭ�顢��ɫ��ͨ������
	auto channel = new GameChannel(_fd);
	auto protol = new GameProtol();
	auto role = new GameRole();

	//ͨ����Э���
	protol->m_channel = channel;
	channel->m_proto = protol;

	//��ɫ��Э���
	protol->m_role = role;
	role->m_proto = protol;

	//��Э��ͽ�ɫ������ӽ��ں�
	ZinxKernel::Zinx_Add_Role(*role);
	ZinxKernel::Zinx_Add_Proto(*protol);
	return channel;
}
