
#include "stdafx.h"
#include "LoginServer.h"
#include "Network/Service/SubServerConnector.h"

#include "NetProtocol/Src/login_Protocol.cpp"
#include "NetProtocol/Src/login_ProtocolListener.cpp"
#include "NetProtocol/Src/server_network_Protocol.cpp"
#include "NetProtocol/Src/server_network_ProtocolListener.cpp"
#include "NetProtocol/src/certify_Protocol.cpp"
#include "NetProtocol/src/certify_ProtocolListener.cpp"

#include "Network/ProtocolHandler/BasicC2SProtocolHandler.h"
#include "BasicC2SProtocolHandler_LoginSvr.h"

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

	MultiPlugPtr pLobbySvrController = multinetwork::CMultiNetwork::Get()->GetController("lobbysvr");
	if (!pLobbySvrController)
	{
		clog::Error( clog::ERROR_CRITICAL, "CLoginServer Init Error!! not found lobbysvr netgroupcontroller" );
		return;
	}
	MultiPlugPtr pCertifySvrController = multinetwork::CMultiNetwork::Get()->GetController("certifysvr");
	if (!pCertifySvrController)
	{
		clog::Error( clog::ERROR_CRITICAL, "CLoginServer Init Error!! not found certify netgroupcontroller" );
		return;
	}

	AddChild( pLobbySvrController );
	AddChild( pCertifySvrController );

	GetServer()->SetOption( true );

	RegisterProtocol(&m_BasicProtocol);
	RegisterProtocol(&m_LoginProtocol);

	m_pBasicPrtHandler = new CBasicC2SProtocolHandler_LoginSvr(*pCertifySvrController, *GetServer());
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
	MultiPlugPtr pLobbySvrController = multinetwork::CMultiNetwork::Get()->GetController("lobbysvr");
	if (!pLobbySvrController)
	{
		clog::Error( clog::ERROR_CRITICAL, "OnSubServerConnect Error!! not found lobbysvr netgroupcontroller" );
		return;
	}
	MultiPlugPtr pCertifySvrController = multinetwork::CMultiNetwork::Get()->GetController("certifysvr");
	if (!pCertifySvrController)
	{
		clog::Error( clog::ERROR_CRITICAL, "OnSubServerConnect Error!! not found certify netgroupcontroller" );
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
	const netid &userId, const std::string &ip, const int &port )
{
	
	return true;
}


/**
 @brief AckMoveUser
 */
bool CLoginServer::AckMoveUser(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode,
	const std::string &id, const netid &userId, const std::string &ip, const int &port)
{
	if (error::ERR_SUCCESS == errorCode)
	{
		m_BasicProtocol.AckMoveToServer( userId, SEND_T, error::ERR_SUCCESS, "lobbysvr", ip, port);
	}
	else
	{
		m_BasicProtocol.AckMoveToServer( userId, SEND_T, errorCode, "lobbysvr", " ", 0);
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

	CSubServerConnector *pSubSvrCon = dynamic_cast<CSubServerConnector*>(pLobbySvrDelegation.Get());
	if (!pSubSvrCon)
	{
		m_LoginProtocol.AckLobbyIn( senderId, SEND_T, error::ERR_REQLOBBYIN_NOTFOUND_SERVER );
		return false;
	}

	std::list<SSubServerInfo> subServers = pSubSvrCon->GetSubServerInfo();
	subServers.sort();
	if (subServers.empty())
	{
		m_LoginProtocol.AckLobbyIn( senderId, SEND_T, error::ERR_REQLOBBYIN_NOTFOUND_SERVER );
		return false;
	}

	SSubServerInfo targetSvr = subServers.front();
	pSubSvrCon->RegisterProtocol( &m_SvrNetworkProtocol );
	m_SvrNetworkProtocol.ReqMoveUser( targetSvr.serverId, SEND_T, 
		pClient->GetName(), pClient->GetNetId(), targetSvr.ip, targetSvr.portnum  );

	return true; 
}
