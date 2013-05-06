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
	virtual bool AckUserLogin(certify::AckUserLogin_Packet &packet) override;

	// client
	virtual bool ReqLogIn(basic::ReqLogIn_Packet &packet) override;

private:
	basic::s2c_Protocol		m_BasicProtocol;
	certify::s2s_Protocol	m_CertifyProtocol;
};
