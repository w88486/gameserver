#include "GameProtol.h"
#include"GameMsg.h"
//待完善：出错处理
UserData* GameProtol::raw2request(std::string _szInput)
{
	MutiMsg *ret = new MutiMsg();
	//缓存数据
	m_beforebuf.append(_szInput);
	//处理多条粘包数据
	while (true) {
		//不足8个字节，无效数据
		if (m_beforebuf.size()<8) {
			break;
		}
		//字节小端数据转大端整型，获取长度
		int len=0;
		len |= m_beforebuf[0];
		len |= m_beforebuf[1]<<8;
		len |= m_beforebuf[2]<<16;
		len |= m_beforebuf[3]<<24;
		//剩余字符小于长度，无效数据
		if (m_beforebuf.size() - 8 < len) {
			break;
		}
		//取类型
		int id=0;
		id |= m_beforebuf[4];
		id |= m_beforebuf[5] << 8;
		id |= m_beforebuf[6] << 16;
		id |= m_beforebuf[7] << 24;
		//添加数据
		auto msg = new GameMsg((GameMsg::MSG_TYPE)id,m_beforebuf.substr(8,len));
		ret->MsgList.push_back(msg);
		//将删除已取的完整数据，剩余数据缓存
		m_beforebuf.erase(0,8+len);
	}
	return ret;
}

std::string* GameProtol::response2raw(UserData& _oUserData)
{
	//从数据中取出长度、类型、和消息序列化的内容
	string *ret = new string();
	string MsgContent;
	GET_REF2DATA(GameMsg, gameData, _oUserData);
	int id = gameData.Type;
	MsgContent = gameData.serialize();
	int len = MsgContent.size();
	//将长度、类型转化成小端字节
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
