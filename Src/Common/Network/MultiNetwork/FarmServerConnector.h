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

#include "MultiNetworkUtility.h"
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
		virtual bool AckSubServerLogin(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode) override;
		virtual bool AckSendSubServerP2PSLink(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode) override;
		virtual bool AckSendSubServerP2PCLink(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode) override;
		virtual bool AckSendSubServerInputLink(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode) override;
		virtual bool AckSendSubServerOutputLink(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode) override;
		virtual bool AckServerInfoList(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode, const std::string &clientSvrType, const std::string &serverSvrType, const std::vector<SHostInfo> &v) override;
		virtual bool AckToBindOuterPort(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode, const std::string &bindSubServerSvrType, const int &port) override;
		virtual bool AckToBindInnerPort(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode, const std::string &bindSubServerSvrType, const int &port)override;
		virtual bool AckSubServerBindComplete(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode, const std::string &subServerSvrType) override;
		virtual bool AckSubClientConnectComplete(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode, const std::string &subClientSvrType) override;
		virtual bool BindSubServer(IProtocolDispatcher &dispatcher, netid senderId, const std::string &bindSubSvrType, const std::string &ip, const int &port) override;

	private:
		farm::c2s_Protocol m_Protocol;
		SSvrConfigData m_Config;
		bool	m_IsDetectedSendConfig;

	};

}}
