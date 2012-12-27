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
//#include "S2CProtocol.h"
#include "s2c_Protocol.h"
#include "c2s_Protocol.h"

class CChatServer : public network::CServer, public c2s_ProtocolListener
{
public:
	CChatServer()
	{
		RegisterProtocol(&m_S2CProtocol);
		AddListener( this );
	}

protected:
	s2c_Protocol m_S2CProtocol;

public:
	virtual void func2(netid senderId, const std::string &str) override
	{
		std::string sndStr = "server send ";
		sndStr += str;
		m_S2CProtocol.func2(senderId, sndStr);
	}
	virtual void func3(netid senderId, const float &value) override
	{
		int a = 0;
	}

};
