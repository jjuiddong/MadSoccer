/**
Name:   BasicS2CHandler.h
Author:  jjuiddong
Date:    3/17/2013

	CBasicS2CHandler
*/
#pragma once

#include "NetProtocol/Src/basic_ProtocolListener.h"
#include "NetProtocol/Src/basic_Protocol.h"

namespace network
{
	class CClient;
	class CBasicS2CHandler : public common::CEventHandler
											, public basic::s2c_ProtocolListener
	{
	public:
		CBasicS2CHandler( CClient &svr );
		virtual ~CBasicS2CHandler();

	protected:
		void		OnDisconnectClient(CNetEvent &event);

		// Network Handler
		virtual bool AckP2PConnect(IProtocolDispatcher &dispatcher, netid senderId, const network::error::ERROR_CODE &errorCode, const network::P2P_STATE &state, const std::string &ip, const int &port) override;
		virtual bool AckMoveToServer(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode, const std::string &serverName, const std::string &ip, const int &port) override;

	private:
		CClient							&m_Client;		/// CClient Reference 
		basic::c2s_Protocol		m_BasicProtocol;

		bool  m_IsMoveToServer;
		std::string m_ToServerIp;
		int m_ToServerPort;
	};

}
