/**
Name:   LoginServer.h
Author:  jjuiddong
Date:    4/20/2013

 CLoginServer
*/
#pragma once

#include "NetProtocol/Src/basic_Protocol.h"
#include "NetProtocol/Src/login_Protocol.h"
#include "NetProtocol/Src/login_ProtocolListener.h"
#include "NetProtocol/Src/server_network_Protocol.h"
#include "NetProtocol/Src/server_network_ProtocolListener.h"
#include "NetProtocol/src/certify_Protocol.h"
#include "NetProtocol/src/certify_ProtocolListener.h"

class CBasicC2SHandler_LoginSvr;

DECLARE_TYPE_NAME(CLoginServer)
class CLoginServer : public network::CServer
								, public login::c2s_ProtocolListener
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
	virtual void	OnConnectMultiPlug() override;
	void				OnSubServerConnect(network::CNetEvent &event);

	// Network Protocol Handler
	virtual bool ReqLobbyIn(network::IProtocolDispatcher &dispatcher, netid senderId) override;

	// common
	virtual bool ReqMoveUser(network::IProtocolDispatcher &dispatcher, netid senderId, const std::string &id, const certify_key &c_key, const std::string &ip, const int &port) override;
	virtual bool AckMoveUser(network::IProtocolDispatcher &dispatcher, netid senderId, const network::error::ERROR_CODE &errorCode, const std::string &id, const std::string &ip, const int &port) override;

private:
	CBasicC2SHandler_LoginSvr *m_pBasicPrtHandler;
	

	// protocol
	basic::s2c_Protocol m_BasicProtocol;
	login::s2c_Protocol m_LoginProtocol;
	certify::s2s_Protocol m_CertifyProtocol;
	server_network::s2s_Protocol m_SvrNetworkProtocol;

};
