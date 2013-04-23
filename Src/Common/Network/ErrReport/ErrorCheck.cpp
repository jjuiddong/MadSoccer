
#include "stdafx.h"
#include "ErrorCheck.h"

#include "NetProtocol/Src/basic_Protocol.h"

using namespace network;


/**
 @brief ClientCheck
	clientId�� �ش��ϴ� Ŭ���̾�Ʈ�� ���ٸ�, ���� �޼����� Ŭ���̾�Ʈ���� ������ NULL�� �����Ѵ�.
 */
CRemoteClient* network::CheckClient( ServerBasicPtr pServer, basic::s2c_Protocol *pProtocol, netid clientId )
{
	RETV(!pServer, NULL);

	CRemoteClient *pClient = pServer->GetRemoteClient(clientId);
	if (!pClient)
	{
		clog::Error( clog::ERROR_PROBLEM, "Client Check Error!! client not found id=%d", clientId);
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
CRemoteClient* CheckClient( ServerBasicPtr pServer, basic::s2c_Protocol *pProtocol, const std::string &id, netid clientId )
{
	RETV(!pServer, NULL);

	CRemoteClient *pClient = pServer->GetRemoteClient(id);
	if (!pClient)
	{
		clog::Error( clog::ERROR_PROBLEM, "Client Check Error!! client not found id=%s", id.c_str());
		if (pProtocol)
			pProtocol->Error( clientId, SEND_T, error::ERR_NOT_FOUND_USER );
		return NULL;
	}
	return pClient;
}
