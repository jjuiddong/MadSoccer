/**
Name:   CertifyServer.h
Author:  jjuiddong
Date:    4/18/2013

 인증 서버
*/
#pragma once

#include "NetProtocol/src/certify_Protocol.h"
#include "NetProtocol/src/certify_ProtocolListener.h"

DECLARE_TYPE_NAME(CCertifyServer)
class CCertifyServer : public network::CServer
								  ,public certify::s2s_ProtocolListener

								  ,public network::AllProtocolDisplayer
								  ,public memmonitor::Monitor<CCertifyServer, TYPE_NAME(CCertifyServer)>
{
public:
	CCertifyServer();
	virtual ~CCertifyServer();

protected:
	// Event Handler
	virtual void	OnConnectNetGroupController() override;

	// Network Protocol Handler
	virtual void ReqUserId(netid senderId, const std::string &id, const std::string &passwd) override;

private:
	certify::s2s_Protocol m_Protocol;

};
