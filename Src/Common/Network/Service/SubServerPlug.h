/**
Name:   SubServerPlug.h
Author:  jjuiddong
Date:    4/25/2013

	������ �ٸ� ���� �����鰣�� ����� �ϴ� ��ü.
*/
#pragma once

#include "NetProtocol/Src/server_network_Protocol.h"
#include "NetProtocol/Src/server_network_ProtocolListener.h"


namespace network
{
	class CSubServerPlug : public multinetwork::CMultiPlugDelegation
										, public server_network::s2s_ProtocolListener
	{
	public:
		CSubServerPlug();
		virtual ~CSubServerPlug();

		std::list<SSubServerInfo> GetSubServerInfo();

	protected:
		// Event
		virtual void	OnConnectMultiPlug() override;

		/// Network Protocol Handler
		virtual bool SendServerInfo(IProtocolDispatcher &dispatcher, netid senderId, const std::string &svrType, const std::string &ip, const int &port, const int &userCount) override;

	private:
		typedef common::StableVectorMap<netid, CRemoteServer*>	RemoteServers;
		RemoteServers m_RemoteServers;

	};

}
