/**
Name:   BasicC2SHandler_LobbySvr.h
Author:  jjuiddong
Date:    5/1/2013


*/
#pragma once

#include "Network/ProtocolHandler/BasicC2SProtocolHandler.h"

#include "NetProtocol/Src/server_network_Protocol.h"
#include "NetProtocol/src/certify_Protocol.h"
#include "NetProtocol/src/certify_ProtocolListener.h"


class CBasicC2SHandler_LobbySvr : public network::CBasicC2SProtocolHandler
																	, public certify::s2s_ProtocolListener
{
public:
	CBasicC2SHandler_LobbySvr(network::multinetwork::CMultiPlug &certifySvr, network::CServerBasic &svr);
	virtual ~CBasicC2SHandler_LobbySvr();

protected:
	// Network Protocol Handler
	// certify server
	virtual bool AckUserId(network::IProtocolDispatcher &dispatcher, netid senderId, const network::error::ERROR_CODE &errorCode, const std::string &id, const netid &userId) override;

	// client
	virtual bool ReqLogIn(network::IProtocolDispatcher &dispatcher, netid senderId, const std::string &id, const std::string &passwd) override;

private:
	basic::s2c_Protocol		m_BasicProtocol;
	certify::s2s_Protocol	m_CertifyProtocol;
};
