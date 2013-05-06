
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
bool CBasicC2SHandler_LobbySvr::ReqLogIn( basic::ReqLogIn_Packet &packet)
{
	if (!CBasicC2SHandler::ReqLogIn(packet))
		return false;

	CSession *pClient = network::CheckClientNetId( &GetServer(), packet.senderId, &m_BasicProtocol, packet.pdispatcher );
	RETV(!pClient, false);

	MultiPlugDelegationPtr pLobbySvrDelegation = CheckDelegation( "client", packet.senderId, &m_BasicProtocol, packet.pdispatcher );
	RETV(!pLobbySvrDelegation, false);

	CLobbyServer *pLobbySvr = CheckCasting<CLobbyServer*>( pLobbySvrDelegation.Get(), packet.senderId, &m_BasicProtocol, packet.pdispatcher );
	RETV(!pLobbySvr, false);

	PlayerPtr pUser = GetServer().GetPlayer(packet.id);
	if (!pUser)
	{
		clog::Error( clog::ERROR_CRITICAL, 0, "ReqLogIn Error!! not found user id=%s", packet.id.c_str());
		m_BasicProtocol.AckLogIn( packet.senderId, SEND_T, error::ERR_NOT_FOUND_USER, packet.id, packet.c_key);
		return false;
	}

	// check certify key
	if (pUser->GetCertifyKey() != packet.c_key)
	{
		clog::Error( clog::ERROR_PROBLEM, 0, "ReqLogIn Error!! invalid certify key key=%d", packet.c_key );
		m_BasicProtocol.AckLogIn( packet.senderId, SEND_T, error::ERR_INVALID_CERTIFY_KEY, packet.id, packet.c_key);
		return false;
	}
	
	pClient->SetCertifyKey(packet.c_key);
	m_CertifyProtocol.ReqUserMoveServer( SERVER_NETID, SEND_T, packet.id, "lobbysvr" );
	m_BasicProtocol.AckLogIn( packet.senderId, SEND_T, error::ERR_SUCCESS, packet.id, packet.c_key);
	return true;
}


/**
 @brief AckUserId
 */
bool CBasicC2SHandler_LobbySvr::AckUserLogin(certify::AckUserLogin_Packet &packet)
{
	CSession *pClient = network::CheckClientId( &GetServer(), packet.id, 0, &m_BasicProtocol, packet.pdispatcher );
	RETV(!pClient, false);

	if (packet.errorCode != error::ERR_SUCCESS)
	{
		clog::Error( clog::ERROR_PROBLEM, "AckUserId Error!! client generate user id Error id=%s", 
			packet.id.c_str());
		return false;
	}

	pClient->SetState(SESSIONSTATE_LOGIN); // login state
	m_BasicProtocol.AckLogIn(pClient->GetNetId(), SEND_T, packet.errorCode, packet.id, 0);
	return true;
}
