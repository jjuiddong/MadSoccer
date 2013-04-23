
#include "stdafx.h"
#include "ErrorCheck.h"

#include "NetProtocol/Src/basic_Protocol.h"

using namespace network;


/**
 @brief ClientCheck
	clientId에 해당하는 클라이언트가 없다면, 에러 메세지를 클라이언트에게 보내고 NULL을 리턴한다.
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
  id 에 해당하는 클라이언트가 없다면, 에러 메세지를 클라이언트에게 보내고 NULL을 리턴한다.
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
