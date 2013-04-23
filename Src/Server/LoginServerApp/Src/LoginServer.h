/**
Name:   LoginServer.h
Author:  jjuiddong
Date:    4/20/2013

 CLoginServer
*/
#pragma once

#include "NetProtocol/Src/login_Protocol.h"
#include "NetProtocol/Src/login_ProtocolListener.h"
#include "NetProtocol/Src/server_network_Protocol.h"
#include "NetProtocol/Src/server_network_ProtocolListener.h"
#include "NetProtocol/src/certify_Protocol.h"
#include "NetProtocol/src/certify_ProtocolListener.h"

class CBasicC2SProtocolHandler_LoginSvr;

DECLARE_TYPE_NAME(CLoginServer)
class CLoginServer : public network::CServer
								//, public login::c2s_ProtocolListener
								//, public network::CBasicC2SProtocolHandler
								, public server_network::s2s_ProtocolListener
								, public certify::s2s_ProtocolListener

								, public network::AllProtocolDisplayer
								, public memmonitor::Monitor<CLoginServer, TYPE_NAME(CLoginServer)>
{
public:
	CLoginServer();
	virtual ~CLoginServer();

protected:
	// Network Event
	virtual void	OnConnectNetGroupController() override;
	void				OnSubServerConnect(network::CNetEvent &event);

	// Network Protocol Handler

	// common
	virtual bool ReqMoveUser(netid senderId, const std::string &id, const netid &userId) override;
	virtual bool AckMoveUser(netid senderId, const network::error::ERROR_CODE &errorCode, const std::string &id, const netid &userId) override;

private:
	login::s2c_Protocol m_LoginProtocol;
	certify::s2s_Protocol m_CertifyProtocol;
	server_network::s2s_Protocol m_SvrNetworkProtocol;
	CBasicC2SProtocolHandler_LoginSvr *m_pBasicPrtHandler;

};
