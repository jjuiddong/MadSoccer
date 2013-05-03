
#include "stdafx.h"
#include "BasicC2SHandler_LobbySvr.h"
#include "LobbyServer.h"


using namespace network;


CBasicC2SHandler_LobbySvr::CBasicC2SHandler_LobbySvr(
	multinetwork::CMultiPlug &certifySvr, CServerBasic &svr) : 
	CBasicC2SHandler(svr)
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
bool CBasicC2SHandler_LobbySvr::ReqLogIn( network::IProtocolDispatcher &dispatcher, 
	netid senderId, const std::string &id, const std::string &passwd, const certify_key &c_key)
{
	if (!CBasicC2SHandler::ReqLogIn(dispatcher, senderId, id, passwd, c_key))
		return false;

	CSession *pClient = network::CheckClientNetId( &GetServer(), senderId, &m_BasicProtocol, &dispatcher );
	RETV(!pClient, false);

	MultiPlugDelegationPtr pLobbySvrDelegation = CheckDelegation( "client", senderId, &m_BasicProtocol, &dispatcher );
	RETV(!pLobbySvrDelegation, false);

	CLobbyServer *pLobbySvr = CheckCasting<CLobbyServer*>( pLobbySvrDelegation.Get(), senderId, &m_BasicProtocol, &dispatcher );
	RETV(!pLobbySvr, false);

	UserPtr pUser = pLobbySvr->GetUser(id);
	if (!pUser)
	{
		clog::Error( clog::ERROR_CRITICAL, 0, "ReqLogIn Error!! not found user id=%s", id.c_str());
		m_BasicProtocol.AckLogIn( senderId, SEND_T, error::ERR_NOT_FOUND_USER, id, c_key);
		return false;
	}

	// check certify key
	if (pUser->GetCertifyKey() != c_key)
	{
		clog::Error( clog::ERROR_PROBLEM, 0, "ReqLogIn Error!! invalid certify key key=%d", c_key );
		m_BasicProtocol.AckLogIn( senderId, SEND_T, error::ERR_INVALID_CERTIFY_KEY, id, c_key);
		return false;
	}
	
	pClient->SetCertifyKey(c_key);
	m_CertifyProtocol.ReqUserMoveServer( SERVER_NETID, SEND_T, id, "lobbysvr" );
	return true;
}


/**
 @brief AckUserId
 */
bool CBasicC2SHandler_LobbySvr::AckUserLogin(IProtocolDispatcher &dispatcher, netid senderId, 
	const error::ERROR_CODE &errorCode, const std::string &id, const certify_key &c_key)
{
	CSession *pClient = network::CheckClientId( &GetServer(), id, 0, &m_BasicProtocol, &dispatcher );
	RETV(!pClient, false);

	if (errorCode != error::ERR_SUCCESS)
	{
		clog::Error( clog::ERROR_PROBLEM, "AckUserId Error!! client generate user id Error id=%s", id.c_str());
		return false;
	}

	pClient->SetState(SESSIONSTATE_LOGIN); // login state
	m_BasicProtocol.AckLogIn(pClient->GetNetId(), SEND_T, errorCode, id, 0);
	return true;
}
