/**
Name:   BasicC2SHandler_LobbySvr.h
Author:  jjuiddong
Date:    5/1/2013

	CBasicC2SHandler_LobbySvr
*/
#pragma once

#include "Network/ProtocolHandler/BasicC2SHandler.h"

#include "NetProtocol/Src/server_network_Protocol.h"
#include "NetProtocol/src/certify_Protocol.h"
#include "NetProtocol/src/certify_ProtocolListener.h"


class CBasicC2SHandler_LobbySvr : public network::CBasicC2SHandler
														, public certify::s2s_ProtocolListener
{
public:
	CBasicC2SHandler_LobbySvr(network::multinetwork::CMultiPlug &certifySvr, network::CServerBasic &svr);
	virtual ~CBasicC2SHandler_LobbySvr();

protected:
	// Network Protocol Handler
	// certify server
	virtual bool AckUserLogin(network::IProtocolDispatcher &dispatcher, netid senderId, const network::error::ERROR_CODE &errorCode, const std::string &id, const certify_key &c_key) override;

	// client
	virtual bool ReqLogIn(network::IProtocolDispatcher &dispatcher, netid senderId, const std::string &id, const std::string &passwd, const certify_key &c_key) override;

private:
	basic::s2c_Protocol		m_BasicProtocol;
	certify::s2s_Protocol	m_CertifyProtocol;
};
