/**
Name:   SubServerConnector.h
Author:  jjuiddong
Date:    4/25/2013

	������ �ٸ� ���� �����鰣�� ����� �ϴ� ��ü.
*/
#pragma once

#include "NetProtocol/Src/server_network_Protocol.h"
#include "NetProtocol/Src/server_network_ProtocolListener.h"


namespace network
{
	class CSubServerConnector : public multinetwork::CNetGroupDelegation
												, public server_network::s2s_ProtocolListener
	{
	public:
		CSubServerConnector();
		virtual ~CSubServerConnector();

		std::list<SSubServerInfo> GetSubServerInfo();

	protected:
		// Event
		virtual void	OnConnectNetGroupController() override;

		/// Network Protocol Handler
		virtual bool SendServerInfo(IProtocolDispatcher &dispatcher, netid senderId, const std::string &svrType, const int &userCount) override;

	private:
	};

}
