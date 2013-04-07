/**
Name:   ServerConnector.h
Author:  jjuiddong
Date:    4/3/2013

FarmServer 에 접속해서, 필요한 정보를 요청하고, 응답을 받는다.

- FarmServer Login 
- p2p, input line, output link 정보를 farm server에 보낸다.
- bind 될 port 값, connect 할 서버의 ip, port 정보를 얻는다.
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
	{
	public:
		CFarmServerConnector( const std::string &svrType );
		virtual ~CFarmServerConnector() {}

		bool		Start( const std::string &ip, const int port, const SSvrConfigData &config );

	protected:
		bool		MakeupInputOutputLink();
		bool		CreateSubController( SERVICE_TYPE serviceType, bool IsInnerBind,
			const std::string &connectSubSvrType, const std::string &bindSubSvrType );

	private:
		// network event handler
		void OnConnect(CNetEvent &event);

		// network packet handler
		virtual void AckSubServerLogin(netid senderId, const error::ERROR_CODE &errorCode) override;
		virtual void AckSendSubServerP2PLink(netid senderId, const error::ERROR_CODE &errorCode) override;
		virtual void AckSendSubServerInputLink(netid senderId, const error::ERROR_CODE &errorCode) override;
		virtual void AckSendSubServerOutputLink(netid senderId, const error::ERROR_CODE &errorCode) override;
		virtual void AckServerInfoList(netid senderId, const error::ERROR_CODE &errorCode, const std::string &clientSvrType, const std::string &serverSvrType, const std::vector<SHostInfo> &v) override;
		virtual void AckToBindOuterPort(netid senderId, const error::ERROR_CODE &errorCode, const std::string &bindSubServerSvrType, const int &port) override;
		virtual void AckToBindInnerPort(netid senderId, const error::ERROR_CODE &errorCode, const std::string &bindSubServerSvrType, const int &port)override;
		virtual void AckSubServerBindComplete(netid senderId, const error::ERROR_CODE &errorCode, const std::string &subServerSvrType) override;
		virtual void AckSubClientConnectComplete(netid senderId, const error::ERROR_CODE &errorCode, const std::string &subClientSvrType) override;

	private:
		farm::c2s_Protocol m_Protocol;
		SSvrConfigData m_Config;
		bool	m_IsDetectedSendConfig;

	};

}}
