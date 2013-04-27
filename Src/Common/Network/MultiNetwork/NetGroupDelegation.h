/**
Name:   NetGroupDelegation.h
Author:  jjuiddong
Date:    4/1/2013

network::multinetwork::CNetGroupDelegation 구현
- Network Group 에 접속하는 Controller를 통해 패킷을 전달받고, 이벤트를 핸들링한다.
*/
#pragma once

namespace network { namespace multinetwork {

	/// Network Group 에 접속하는 Controller를 통해 패킷을 전달받고, 이벤트를 핸들링한다.
	class CNetGroupDelegation : public CNetConnectorLinker
	{
	public:
		CNetGroupDelegation();
		virtual ~CNetGroupDelegation();

		void		SetConnector(NetGroupControllerPtr ptr);
		NetGroupControllerPtr		GetConnector() const;

		CServerBasic* GetServer();
		const CoreClients_V& GetClients();
		CoreClientPtr GetClient(netid netId);
		CoreClientPtr GetClientFromServerNetId(netid serverNetId);

	protected:
		// Event 
		virtual void	OnConnectNetGroupController() { }

	private:
		std::string				m_SvrType;
	};

}}
