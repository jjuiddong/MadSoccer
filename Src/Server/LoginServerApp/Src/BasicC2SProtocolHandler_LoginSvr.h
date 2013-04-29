/**
Name:   BasicC2SProtocolHandler_LoginSvr.h
Author:  jjuiddong
Date:    4/22/2013

	Login server 용 basic protocol 처리 클래스
*/
#pragma once

#include "Network/ProtocolHandler/BasicC2SProtocolHandler.h"

#include "NetProtocol/Src/server_network_Protocol.h"
#include "NetProtocol/src/certify_Protocol.h"
#include "NetProtocol/src/certify_ProtocolListener.h"


class CBasicC2SProtocolHandler_LoginSvr : public network::CBasicC2SProtocolHandler
																	, public certify::s2s_ProtocolListener
{
public:
	CBasicC2SProtocolHandler_LoginSvr(network::multinetwork::CMultiPlug &certifySvr, network::CServerBasic &svr);
	virtual ~CBasicC2SProtocolHandler_LoginSvr();

protected:
	// Network Protocol Handler
	// certify server
	virtual bool AckUserId(network::IProtocolDispatcher &dispatcher, netid senderId, const network::error::ERROR_CODE &errorCode, const std::string &id, const netid &userId) override;

	// client
	virtual bool ReqLogIn(network::IProtocolDispatcher &dispatcher, netid senderId, const std::string &id, const std::string &passwd) override;
	virtual bool ReqLogOut(network::IProtocolDispatcher &dispatcher, netid senderId, const std::string &id) override;
	virtual bool ReqMoveToServer(network::IProtocolDispatcher &dispatcher, netid senderId, const std::string &serverName) override;

private:
	certify::s2s_Protocol	m_CertifyProtocol;
	basic::s2c_Protocol		m_BasicProtocol;
};
