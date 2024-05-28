#include "GameProtol.h"
#include"GameMsg.h"
//�����ƣ�������
UserData* GameProtol::raw2request(std::string _szInput)
{
	MutiMsg *ret = new MutiMsg();
	//��������
	m_beforebuf.append(_szInput);
	//�������ճ������
	while (true) {
		//����8���ֽڣ���Ч����
		if (m_beforebuf.size()<8) {
			break;
		}
		//�ֽ�С������ת������ͣ���ȡ����
		int len=0;
		len |= m_beforebuf[0];
		len |= m_beforebuf[1]<<8;
		len |= m_beforebuf[2]<<16;
		len |= m_beforebuf[3]<<24;
		//ʣ���ַ�С�ڳ��ȣ���Ч����
		if (m_beforebuf.size() - 8 < len) {
			break;
		}
		//ȡ����
		int id=0;
		id |= m_beforebuf[4];
		id |= m_beforebuf[5] << 8;
		id |= m_beforebuf[6] << 16;
		id |= m_beforebuf[7] << 24;
		//�������
		auto msg = new GameMsg((GameMsg::MSG_TYPE)id,m_beforebuf.substr(8,len));
		ret->MsgList.push_back(msg);
		//��ɾ����ȡ���������ݣ�ʣ�����ݻ���
		m_beforebuf.erase(0,8+len);
	}
	return ret;
}

std::string* GameProtol::response2raw(UserData& _oUserData)
{
	//��������ȡ�����ȡ����͡�����Ϣ���л�������
	string *ret = new string();
	string MsgContent;
	GET_REF2DATA(GameMsg, gameData, _oUserData);
	int id = gameData.Type;
	MsgContent = gameData.serialize();
	int len = MsgContent.size();
	//�����ȡ�����ת����С���ֽ�
	ret->push_back(len & 0xff);
	ret->push_back((len>>8) & 0xff);
	ret->push_back((len >> 16) & 0xff);
	ret->push_back((len >> 24) & 0xff);

	ret->push_back(id & 0xff);
	ret->push_back((id >> 8) & 0xff);
	ret->push_back((id >> 16) & 0xff);
	ret->push_back((id >> 24) & 0xff);
	ret->append(MsgContent);
	return ret;
}

Irole* GameProtol::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{

	return m_role;
}

Ichannel* GameProtol::GetMsgSender(BytesMsg& _oBytes)
{
	return nullptr;
}

GameProtol::~GameProtol()
{
	if (NULL != m_role) {
		delete m_role;
	}
}
