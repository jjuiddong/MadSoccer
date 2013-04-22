
#include "stdafx.h"
#include "LoginServer.h"

#include "NetProtocol/Src/login_Protocol.cpp"
#include "NetProtocol/Src/login_ProtocolListener.cpp"
#include "NetProtocol/Src/server_network_Protocol.cpp"
#include "NetProtocol/Src/server_network_ProtocolListener.cpp"
#include "NetProtocol/src/certify_Protocol.cpp"
#include "NetProtocol/src/certify_ProtocolListener.cpp"


using namespace network;


CLoginServer::CLoginServer()
{
}

CLoginServer::~CLoginServer()
{
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

	RegisterProtocol(&m_LoginProtocol);

	AddProtocolListener(this);
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
void CLoginServer::ReqMoveUser(netid senderId, const std::string &id, const netid &userId)
{

}


/**
 @brief AckMoveUser
 */
void CLoginServer::AckMoveUser(netid senderId, const error::ERROR_CODE &errorCode, const std::string &id, const netid &userId)
{

}


/**
 @brief ReqLogIn
 */
void CLoginServer::ReqLogIn(netid senderId, const std::string &id, const std::string &passwd)
{
	CRemoteClient *pClient = GetServer()->GetRemoteClient(id);
	if (pClient)
	{
		clog::Error( clog::ERROR_PROBLEM, "ReqLogin Error!! client already exist senderId=%d, id=%s",
			senderId, id.c_str());
		m_LoginProtocol.AckLogIn(pClient->GetId(), SEND_T, error::ERR_ALREADY_EXIST_USER, id, 0);
		return;
	}

	pClient = GetServer()->GetRemoteClient(senderId);
	if (!pClient)
	{
		clog::Error( clog::ERROR_PROBLEM, "ReqLogin Error!! client not found senderId=%d, id=%s",
			senderId, id.c_str());
		m_LoginProtocol.AckLogIn(pClient->GetId(), SEND_T, error::ERR_NOT_FOUND_USER, id, 0);
		return;
	}

	pClient->SetName(id);

	m_CertifyProtocol.ReqUserId(SERVER_NETID, SEND_T, id, passwd);
}


/**
 @brief ReqLogOut
 */
void CLoginServer::ReqLogOut(netid senderId, const std::string &id)
{

}


/**
 @brief 
 */
void CLoginServer::ReqMoveToServer(netid senderId, const std::string &serverName)
{

}


/**
 @brief 
 */
void CLoginServer::AckUserId(netid senderId, const error::ERROR_CODE &errorCode, const std::string &id, const netid &userId)
{
	CRemoteClient *pClient = GetServer()->GetRemoteClient(id);
	if (!pClient)
	{
		clog::Error( clog::ERROR_PROBLEM, "AckUserId Error!! client not found id=%s", id.c_str());
		return;
	}

	if (errorCode != error::ERR_SUCCESS)
	{
		clog::Error( clog::ERROR_PROBLEM, "AckUserId Error!! client generate user id Error id=%s", id.c_str());
	}

	m_LoginProtocol.AckLogIn(pClient->GetId(), SEND_T, errorCode, id, userId);
}

