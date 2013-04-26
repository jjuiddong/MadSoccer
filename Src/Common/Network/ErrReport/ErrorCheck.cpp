
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
	if (pClient->GetState() == CLIENTSTATE_LOGIN)
		return true;
	if (pDispatcher)
		pDispatcher->PrintThisPacket( clog::LOG_F, "!!! Error client not connection >>" );
	return false;
}

