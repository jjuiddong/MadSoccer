
#include "stdafx.h"
#include "LoginServer.h"

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
void	CLoginServer::OnConnectNetGroupController()
{
	CServer::OnConnectNetGroupController();

	NetGroupControllerPtr pLobbySvrController = multinetwork::CMultiNetwork::Get()->GetController("lobbysvr");
	if (!pLobbySvrController)
	{
		clog::Error( clog::ERROR_CRITICAL, "CLoginServer Init Error!! not found lobbysvr netgroupcontroller" );
		return;
	}
	NetGroupControllerPtr pCertifySvrController = multinetwork::CMultiNetwork::Get()->GetController("certifysvr");
	if (!pCertifySvrController)
	{
		clog::Error( clog::ERROR_CRITICAL, "CLoginServer Init Error!! not found certify netgroupcontroller" );
		return;
	}

	m_pBasicPrtHandler = new CBasicC2SProtocolHandler_LoginSvr(*pCertifySvrController, *GetServer());

	RegisterProtocol(&m_LoginProtocol);

	AddProtocolListener(this);
	AddProtocolListener(m_pBasicPrtHandler);
	pLobbySvrController->AddProtocolListener(this);
	pCertifySvrController->AddProtocolListener(this);

	EVENT_CONNECT_TO(pLobbySvrController, this, EVT_CONNECT, CLoginServer, CLoginServer::OnSubServerConnect);
	EVENT_CONNECT_TO(pCertifySvrController, this, EVT_CONNECT, CLoginServer, CLoginServer::OnSubServerConnect);

}


/**
 @brief OnLobbySvrConnect
 */
void	CLoginServer::OnSubServerConnect(CNetEvent &event)
{
	NetGroupControllerPtr pLobbySvrController = multinetwork::CMultiNetwork::Get()->GetController("lobbysvr");
	if (!pLobbySvrController)
	{
		clog::Error( clog::ERROR_CRITICAL, "OnSubServerConnect Error!! not found lobbysvr netgroupcontroller" );
		return;
	}
	NetGroupControllerPtr pCertifySvrController = multinetwork::CMultiNetwork::Get()->GetController("certifysvr");
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
bool CLoginServer::ReqMoveUser(netid senderId, const std::string &id, const netid &userId)
{
	return true;
}


/**
 @brief AckMoveUser
 */
bool CLoginServer::AckMoveUser(netid senderId, const error::ERROR_CODE &errorCode, const std::string &id, const netid &userId)
{

	return true;
}

