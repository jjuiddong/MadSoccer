
#include "stdafx.h"
#include "ErrorCheck.h"

#include "NetProtocol/Src/basic_Protocol.h"

using namespace network;


/**
 @brief ClientCheck
	clientId�� �ش��ϴ� Ŭ���̾�Ʈ�� ���ٸ�, ���� �޼����� Ŭ���̾�Ʈ���� ������ NULL�� �����Ѵ�.
 */
CSession* network::CheckClientNetId( ServerBasicPtr pServer, netid clientId, 
	basic::s2c_Protocol *pProtocol, IProtocolDispatcher *pDispatcher )
{
	RETV(!pServer, NULL);

	CSession *pClient = pServer->GetSession(clientId);
	if (!pClient)
	{
		//clog::Error( clog::ERROR_PROBLEM, "Client Check Error!! client not found id=%d", clientId);
		if (pDispatcher)
			pDispatcher->PrintThisPacket( clog::LOG_FILE, "!!! Error client not found from netid>>" );
		if (pProtocol)
			pProtocol->Error( clientId, SEND_T, error::ERR_NOT_FOUND_USER );
		return NULL;
	}
	return pClient;
}


/**
 @brief ClientCheck
  id �� �ش��ϴ� Ŭ���̾�Ʈ�� ���ٸ�, ���� �޼����� Ŭ���̾�Ʈ���� ������ NULL�� �����Ѵ�.
 */
CSession* network::CheckClientId( ServerBasicPtr pServer, const std::string &id, netid clientId, 
	basic::s2c_Protocol *pProtocol, IProtocolDispatcher *pDispatcher )
{
	RETV(!pServer, NULL);

	CSession *pClient = pServer->GetSession(id);
	if (!pClient)
	{
		//clog::Error( clog::ERROR_PROBLEM, "Client Check Error!! client not found id=%s", id.c_str());
		if (pDispatcher)
			pDispatcher->PrintThisPacket( clog::LOG_FILE, "!!! Error client not found from id>>" );
		if (pProtocol)
			pProtocol->Error( clientId, SEND_T, error::ERR_NOT_FOUND_USER );
		return NULL;
	}
	return pClient;
}


/**
 @brief CheckClientConnection
 */
bool network::CheckClientConnection( CSession *pClient, 
	basic::s2c_Protocol *pProtocol, IProtocolDispatcher *pDispatcher )
{
	RETV(!pClient, false);
	if (pClient->GetState() == SESSIONSTATE_LOGIN)
		return true;
	if (pDispatcher)
		pDispatcher->PrintThisPacket( clog::LOG_F, "!!! Error client not connection >>" );
	return false;
}


/**
@brief  CheckPlayerWaitAck
			Check Player IsAckWait State
*/
CPlayer* network::CheckPlayerWaitAck(ServerBasicPtr pServer, netid clientId, 
	basic::s2c_Protocol *pProtocol, IProtocolDispatcher *pDispatcher )
{
	RETV(!pServer, NULL);
	PlayerPtr pPlayer = pServer->GetPlayer(clientId);
	if (!pPlayer)
	{
		if (pDispatcher)
			pDispatcher->PrintThisPacket( clog::LOG_FILE, "!!! Error Player Wait Ack err >>" );
		if (clientId && pProtocol)
			pProtocol->Error( clientId, SEND_T, error::ERR_NOT_FOUND_USER);
		return NULL;
	}

	if (pPlayer->IsAckWait())
	{
		if (pDispatcher)
			pDispatcher->PrintThisPacket( clog::LOG_FILE, "!!! Error Player Wait Ack err >>" );
		if (clientId && pProtocol)
			pProtocol->Error( clientId, SEND_T, error::ERR_WAIT_ACK_PLAYER);
		return NULL;
	}

	return pPlayer;
}


/**
@brief  CheckSessionLogin
			Check Session state  SESSIONSTATE_LOGIN
*/
CSession* network::CheckSessionLogin( ServerBasicPtr pServer, netid clientId, 
	basic::s2c_Protocol *pProtocol, IProtocolDispatcher *pDispatcher )
{
	RETV(!pServer, NULL);

	SessionPtr pClient = pServer->GetSession(clientId);
	if (!pClient)
	{
		if (pDispatcher)
			pDispatcher->PrintThisPacket( clog::LOG_FILE, "CheckSessionLogin!!! Error not found Session >>" );
		if (clientId && pProtocol)
			pProtocol->Error( clientId, SEND_T, error::ERR_NOT_FOUND_USER);
		return NULL;
	}

	if (pClient->GetState() != SESSIONSTATE_LOGIN)
	{
		if (pDispatcher)
			pDispatcher->PrintThisPacket( clog::LOG_FILE, "CheckSessionLogin!!! Error Session Not Login >>" );
		if (clientId && pProtocol)
			pProtocol->Error( clientId, SEND_T, error::ERR_NOT_FOUND_USER);
		return NULL;
	}
	return pClient;	
}


/**
@brief  CheckPlayerWaitAck() && CheckSessionLogin()
*/
CPlayer* network::CheckRecvablePlayer(ServerBasicPtr pServer, netid clientId, 
	basic::s2c_Protocol *pProtocol, IProtocolDispatcher *pDispatcher )
{
	PlayerPtr pPlayer = CheckPlayerWaitAck(pServer, clientId, pProtocol, pDispatcher);
	RETV(!pPlayer,NULL);
	SessionPtr pSession = CheckSessionLogin(pServer, clientId, pProtocol, pDispatcher);
	RETV(!pSession, NULL);

	return pPlayer;
}


/**
 @brief CheckDelegation
 */
MultiPlugDelegationPtr network::CheckDelegation( const std::string &linkSvrType,
	netid clientId, basic::s2c_Protocol *pProtocol, IProtocolDispatcher *pDispatcher )
{
	MultiPlugDelegationPtr ptr = multinetwork::CMultiNetwork::Get()->GetDelegation(linkSvrType);
	if (!ptr)
	{
		clog::Error( clog::ERROR_CRITICAL, 0, "CheckDelegation Error!!! linkSvrType=%s", linkSvrType.c_str() );
		if (pDispatcher)
			pDispatcher->PrintThisPacket( clog::LOG_FILE, "!!! Error casting err >>" );
		if (clientId && pProtocol)
			pProtocol->Error( clientId, SEND_T, error::ERR_INTERNAL );
	}
	return ptr;
}


