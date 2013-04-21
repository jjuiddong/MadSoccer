/**
Name:   Server.h
Author:  jjuiddong
Date:    2013-03-12

Group Management
*/
#pragma once

#include "../Controller/ServerBasic.h"

namespace basic { class s2c_Protocol; }

namespace network
{
	class CBasicC2SProtocolHandler;
	class CServer //: public CServerBasic
		: public network::multinetwork::CNetGroupDelegation
	{
		friend class CBasicC2SProtocolHandler;

	public:
		CServer();
		virtual ~CServer();

	protected:
		// Event Handler
		virtual void	OnConnectNetGroupController() override;

		// Network Event Handler
		void OnClientJoin(CNetEvent &event);
		void OnClientLeave(CNetEvent &event);

	private:
		// protocols
		basic::s2c_Protocol		*m_pBasicProtocol;
		CBasicC2SProtocolHandler *m_pBasicPrtHandler;
	};
}
