//------------------------------------------------------------------------
// Name:    ChatServer.h
// Author:  jjuiddong
// Date:    12/21/2012
// 
// 테스트 코드
// 채팅서버
//------------------------------------------------------------------------

#pragma once

#include "C2SProtocolListener.h"
#include "S2CProtocol.h"

class CChatServer : public network::CServer, public CC2SProtocolListener
{
public:
	CChatServer()
	{
		SetProtocol(&m_S2CProtocol);
		AddListener( this );
	}

protected:
	CS2CProtocol m_S2CProtocol;
	virtual void ProcessPacket( const network::CPacket &rcvPacket );

public:
	virtual void func1(netid senderId) override
	{

	}
	virtual void func2(netid senderId, std::string &str) override
	{
		if (!IsExist(senderId))
			return;

		// 클라이언트에게 값을 되돌려 줍니다.
// 		char buf[ 256];
// 		strcpy_s(buf, "server send ");
// 		strcat_s(buf, rcvPacket.GetData());
// 		network::CPacket sendPacket(GetSocket(), buf);
// 		SendAll(sendPacket);

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
