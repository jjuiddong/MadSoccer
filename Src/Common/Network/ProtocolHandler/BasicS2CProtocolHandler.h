/**
Name:   BasicS2CProtocolHandler.h
Author:  jjuiddong
Date:    3/17/2013

*/
#pragma once

#include "NetProtocol/Src/basic_ProtocolListener.h"
#include "NetProtocol/Src/basic_Protocol.h"

namespace network
{
	class CClient;

	class CBasicS2CProtocolHandler : public basic::s2c_ProtocolListener
	{
	public:
		CBasicS2CProtocolHandler( CClient &svr );
		virtual ~CBasicS2CProtocolHandler();

		// Network Handler
		virtual bool AckP2PConnect(IProtocolDispatcher &dispatcher, netid senderId, const network::error::ERROR_CODE &errorCode, const network::P2P_STATE &state, const std::string &ip, const int &port) override;

	private:
		CClient							&m_Client;		/// CClient Reference 
		basic::c2s_Protocol		m_BasicProtocol;
	};

}
