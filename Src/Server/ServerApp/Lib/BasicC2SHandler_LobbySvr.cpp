
#include "stdafx.h"
#include "BasicC2SHandler_LobbySvr.h"

using namespace network;


CBasicC2SHandler_LobbySvr::CBasicC2SHandler_LobbySvr(
	multinetwork::CMultiPlug &certifySvr, CServerBasic &svr) : 
	CBasicC2SProtocolHandler(svr)
{
	svr.RegisterProtocol(&m_BasicProtocol);
	certifySvr.RegisterProtocol(&m_CertifyProtocol);
}

CBasicC2SHandler_LobbySvr::~CBasicC2SHandler_LobbySvr()
{
}


/**
 @brief ReqLogIn
 */
bool CBasicC2SHandler_LobbySvr::ReqLogIn(
	network::IProtocolDispatcher &dispatcher, netid senderId, const std::string &id, const std::string &passwd)
{
	if (!CBasicC2SProtocolHandler::ReqLogIn(dispatcher, senderId, id, passwd))
		return false;

	m_CertifyProtocol.ReqUserId(SERVER_NETID, SEND_T, id, passwd);	
	return true;
}


/**
 @brief AckUserId
 */
bool CBasicC2SHandler_LobbySvr::AckUserId(
	IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode, 
	const std::string &id, const netid &userId)
{
	CSession *pClient = network::CheckClientId( &GetServer(), id, userId, &m_BasicProtocol, &dispatcher );
	RETV(!pClient, false);

	if (errorCode != error::ERR_SUCCESS)
	{
		clog::Error( clog::ERROR_PROBLEM, "AckUserId Error!! client generate user id Error id=%s", id.c_str());
		return false;
	}

	pClient->SetState(SESSIONSTATE_LOGIN); // login state
	m_BasicProtocol.AckLogIn(pClient->GetNetId(), SEND_T, errorCode, id, userId);
	return true;
}
	