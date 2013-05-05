
#include "stdafx.h"
#include "LoginServer.h"
#include "Network/Service/SubServerPlug.h"

#include "Network/ProtocolHandler/BasicC2SHandler.h"
#include "BasicC2SHandler_LoginSvr.h"


using namespace network;


CLoginServer::CLoginServer() :
	m_pBasicPrtHandler(NULL)
{
}

CLoginServer::~CLoginServer()
{
	SAFE_DELETE(m_pBasicPrtHandler);

}


/**
 @brief OnConnectNetGroupController
 */
void	CLoginServer::OnConnectMultiPlug()
{
	CServer::OnConnectMultiPlug();

	MultiPlugPtr pLobbySvrController = multinetwork::CMultiNetwork::Get()->GetMultiPlug("lobbysvr");
	if (!pLobbySvrController)
	{
		clog::Error( clog::ERROR_CRITICAL, "CLoginServer Init Error!! not found lobbysvr multiplug" );
		return;
	}
	MultiPlugPtr pCertifySvrController = multinetwork::CMultiNetwork::Get()->GetMultiPlug("certifysvr");
	if (!pCertifySvrController)
	{
		clog::Error( clog::ERROR_CRITICAL, "CLoginServer Init Error!! not found certify multiplug" );
		return;
	}

	AddChild( pLobbySvrController );
	AddChild( pCertifySvrController );

	GetServer()->SetOption( true );

	RegisterProtocol(&m_BasicProtocol);
	RegisterProtocol(&m_LoginProtocol);

	m_pBasicPrtHandler = new CBasicC2SHandler_LoginSvr(*pCertifySvrController, *GetServer());
	AddProtocolListener(this);
	AddProtocolListener(m_pBasicPrtHandler);

	//pLobbySvrController->AddProtocolListener(this);
	//pCertifySvrController->AddProtocolListener(this);

	NETEVENT_CONNECT(EVT_CONNECT, CLoginServer, CLoginServer::OnSubServerConnect);
	NETEVENT_CONNECT(EVT_CONNECT, CLoginServer, CLoginServer::OnSubServerConnect);

}


/**
 @brief OnLobbySvrConnect
 */
void	CLoginServer::OnSubServerConnect(CNetEvent &event)
{
	MultiPlugPtr pLobbySvrController = multinetwork::CMultiNetwork::Get()->GetMultiPlug("lobbysvr");
	if (!pLobbySvrController)
	{
		clog::Error( clog::ERROR_CRITICAL, "OnSubServerConnect Error!! not found lobbysvr multiplug" );
		return;
	}
	MultiPlugPtr pCertifySvrController = multinetwork::CMultiNetwork::Get()->GetMultiPlug("certifysvr");
	if (!pCertifySvrController)
	{
		clog::Error( clog::ERROR_CRITICAL, "OnSubServerConnect Error!! not found certify multiplug" );
		return;
	}

	if (pLobbySvrController->IsConnect())
		pLobbySvrController->RegisterProtocol(&m_SvrNetworkProtocol);
	if (pCertifySvrController->IsConnect())
		pCertifySvrController->RegisterProtocol(&m_CertifyProtocol);
}


/**
 @brief ReqMoveUser
 */
bool CLoginServer::ReqMoveUser(IProtocolDispatcher &dispatcher, netid senderId, const std::string &id, 
	const certify_key &c_key, const std::string &ip, const int &port )
{
	
	return true;
}


/**
 @brief AckMoveUser
 */
bool CLoginServer::AckMoveUser(IProtocolDispatcher &dispatcher, netid senderId, 
	const error::ERROR_CODE &errorCode, const std::string &id, const std::string &ip, const int &port)
{
	CSession *pClient = CheckClientId(GetServer(), id, 0, NULL, NULL);
	if (!pClient)
	{
		clog::Error( clog::ERROR_CRITICAL, "AckMoveUser Error!! not found client id = %s", id.c_str() );
		m_SvrNetworkProtocol.ReqMoveUserCancel( senderId, SEND_T, id );
		return false;
	}

	if (error::ERR_SUCCESS == errorCode)
	{
		m_BasicProtocol.AckMoveToServer( pClient->GetNetId(), SEND_T, error::ERR_SUCCESS, "lobbysvr", ip, port);
	}
	else
	{
		m_BasicProtocol.AckMoveToServer( pClient->GetNetId(), SEND_T, errorCode, "lobbysvr", " ", 0);
	}
	return true;
}


/**
 @brief ReqLobbyIn
 로비서버로 이동 요청 
 */
bool CLoginServer::ReqLobbyIn(IProtocolDispatcher &dispatcher, netid senderId)
{
	CSession *pClient = CheckClientNetId(GetServer(), senderId, &m_BasicProtocol, &dispatcher);
	RETV(!pClient, false);

	if (!CheckClientConnection(pClient, &m_BasicProtocol, &dispatcher))
		return false;

	MultiPlugDelegationPtr pLobbySvrDelegation = multinetwork::CMultiNetwork::Get()->GetDelegation("lobbysvr");
	if (!pLobbySvrDelegation)
	{
		clog::Error( clog::ERROR_CRITICAL, "ReqLobbyIn Error!! not found lobbysvr netgroupdelegation" );
		m_LoginProtocol.AckLobbyIn( senderId, SEND_T, error::ERR_REQLOBBYIN_NOTFOUND_SERVER );
		return false;
	}

	CSubServerPlug *pSubSvrCon = dynamic_cast<CSubServerPlug*>(pLobbySvrDelegation.Get());
	if (!pSubSvrCon)
	{
		clog::Error( clog::ERROR_CRITICAL, "ReqLobbyIn Error!! CSubServerConnector convert error" );
		m_LoginProtocol.AckLobbyIn( senderId, SEND_T, error::ERR_REQLOBBYIN_NOTFOUND_SERVER );
		return false;
	}

	std::list<SSubServerInfo> subServers = pSubSvrCon->GetSubServerInfo();
	subServers.sort();
	if (subServers.empty())
	{
		clog::Error( clog::ERROR_CRITICAL, "ReqLobbyIn Error!! not found lobbysvr server" );
		m_LoginProtocol.AckLobbyIn( senderId, SEND_T, error::ERR_REQLOBBYIN_NOTFOUND_SERVER );
		return false;
	}

	SSubServerInfo targetSvr = subServers.front();
	pSubSvrCon->RegisterProtocol( &m_SvrNetworkProtocol );
	m_SvrNetworkProtocol.ReqMoveUser( targetSvr.serverId, SEND_T, 
		pClient->GetName(), pClient->GetCertifyKey(), targetSvr.ip, targetSvr.portnum  );

	return true; 
}
