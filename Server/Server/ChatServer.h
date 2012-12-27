//------------------------------------------------------------------------
// Name:    ChatServer.h
// Author:  jjuiddong
// Date:    12/21/2012
// 
// 테스트 코드
// 채팅서버
//------------------------------------------------------------------------

#pragma once

#include "C2SProtocol.h"
#include "S2CProtocol.h"

class CChatServer : public network::CServer, public C2SProtocolListener
{
public:
	CChatServer()
	{
		RegisterProtocol(&m_S2CProtocol);
		AddListener( this );
	}

protected:
	S2CProtocol m_S2CProtocol;

public:
	virtual void func1(netid senderId) override
	{

	}
	virtual void func2(netid senderId, std::string &str) override
	{
		std::string sndStr = "server send ";
		sndStr += str;
		m_S2CProtocol.func2(senderId, sndStr);
	}
	virtual void func3(netid senderId, float value) override
	{
		int a = 0;
	}
	virtual void func4(netid senderId) override
	{
		int a = 0;
	}

};
