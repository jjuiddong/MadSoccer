/**
Name:   ServerConnector.h
Author:  jjuiddong
Date:    4/3/2013

FarmServer �� �����ؼ�, �ʿ��� ������ ��û�ϰ�, ������ �޴´�.

- FarmServer Login 
- p2p, input line, output link ������ farm server�� ������.
- bind �� port ��, connect �� ������ ip, port ������ ��´�.
*/
#pragma once

#include "MultiNetworkUtillity.h"
#include "NetGroupDelegation.h"
#include "NetProtocol/Src/farm_Protocol.h"
#include "NetProtocol/Src/farm_ProtocolListener.h"

namespace network { namespace multinetwork {

	struct SSvrConfigData;
	class CFarmServerConnector : public CNetGroupDelegation
													,public farm::s2c_ProtocolListener
													,public AllProtocolDisplayer
	{
	public:
		CFarmServerConnector( const std::string &svrType, const SSvrConfigData &config );
		virtual ~CFarmServerConnector() {}

		bool		Start( const std::string &ip, const int port );

	private:
		void		CreateLink();
		void		ConnectLink();
		bool		CreateSubController( SERVICE_TYPE serviceType, bool IsInnerBind,
			const std::string &connectSubSvrType, const std::string &bindSubSvrType );
		void		ConnectSubController(SERVICE_TYPE serviceType, bool IsInnerBind,
			const std::string &connectSubSvrType, const std::string &bindSubSvrType );

		// Event Handling
		virtual void	OnConnectNetGroupController() override;

		// network event handler
		void OnConnect(CNetEvent &event);
		void OnConnectLink(CNetEvent &event);
		void OnDisconnectLink(CNetEvent &event);

		// network packet handler
		virtual void AckSubServerLogin(netid senderId, const error::ERROR_CODE &errorCode) override;
		virtual void AckSendSubServerP2PSLink(netid senderId, const error::ERROR_CODE &errorCode) override;
		virtual void AckSendSubServerP2PCLink(netid senderId, const error::ERROR_CODE &errorCode) override;
		virtual void AckSendSubServerInputLink(netid senderId, const error::ERROR_CODE &errorCode) override;
		virtual void AckSendSubServerOutputLink(netid senderId, const error::ERROR_CODE &errorCode) override;
		virtual void AckServerInfoList(netid senderId, const error::ERROR_CODE &errorCode, const std::string &clientSvrType, const std::string &serverSvrType, const std::vector<SHostInfo> &v) override;
		virtual void AckToBindOuterPort(netid senderId, const error::ERROR_CODE &errorCode, const std::string &bindSubServerSvrType, const int &port) override;
		virtual void AckToBindInnerPort(netid senderId, const error::ERROR_CODE &errorCode, const std::string &bindSubServerSvrType, const int &port)override;
		virtual void AckSubServerBindComplete(netid senderId, const error::ERROR_CODE &errorCode, const std::string &subServerSvrType) override;
		virtual void AckSubClientConnectComplete(netid senderId, const error::ERROR_CODE &errorCode, const std::string &subClientSvrType) override;
		virtual void BindSubServer(netid senderId, const std::string &bindSubSvrType, const std::string &ip, const int &port) override;

	private:
		farm::c2s_Protocol m_Protocol;
		SSvrConfigData m_Config;
		bool	m_IsDetectedSendConfig;

	};

}}
