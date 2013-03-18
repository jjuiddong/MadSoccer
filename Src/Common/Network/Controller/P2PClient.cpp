
#include "stdafx.h"
#include "P2PClient.h"
#include "CoreClient.h"
#include "NetController.h"

using namespace network;

CP2PClient::CP2PClient(PROCESS_TYPE procType) :
	m_ProcessType(procType)
,	m_pP2pClient(NULL)
,	m_pP2pHost(NULL)
,	m_pEventListener(NULL)
{

}

CP2PClient::~CP2PClient()
{
	Clear();
}


/**
 @brief Create P2P Host Client 
 */
bool	CP2PClient::Bind( const int port )
{
	Stop();

	m_State = P2P_HOST;
	const bool result = CreateP2PHost(port);
	return result;
}


//------------------------------------------------------------------------
// P2P 통신을 시도한다.
// Client = P2P Client 상태가 되고, ip, port 로 접속을 시도한다.
//------------------------------------------------------------------------
bool	CP2PClient::Connect( const std::string &ip, const int port )
{
	Stop();

	m_State = P2P_CLIENT;
	const bool result = CreateP2PClient(ip,port);
	return result;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool	CP2PClient::Stop()
{
	CNetController::Get()->StopServer(m_pP2pHost);
	CNetController::Get()->StopCoreClient(m_pP2pClient);
	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool	CP2PClient::Proc()
{
	switch (m_State)
	{
	case P2P_HOST: 
		if (m_pP2pHost)
			m_pP2pHost->Proc();
		break;

	case P2P_CLIENT:
		if (m_pP2pClient)
			m_pP2pClient->Proc();
		break;
	}

	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void	CP2PClient::Clear()
{
	CNetController::Get()->StopServer(m_pP2pHost);
	CNetController::Get()->StopCoreClient(m_pP2pClient);
	SAFE_DELETE(m_pP2pHost);
	SAFE_DELETE(m_pP2pClient);
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool	CP2PClient::Send(netid netId, const CPacket &packet)
{

	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool	CP2PClient::SendAll(const CPacket &packet)
{

	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool	CP2PClient::CreateP2PHost( const int port )
{
	if (m_pP2pHost)
		CNetController::Get()->StopServer(m_pP2pHost);
	else
		m_pP2pHost = new CServerBasic(m_ProcessType);
	m_pP2pHost->SetEventListener(this);
	return CNetController::Get()->StartServer(port, m_pP2pHost);;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool	CP2PClient::CreateP2PClient( const std::string &ip, const int port )
{
	if (m_pP2pClient)
		CNetController::Get()->StopCoreClient(m_pP2pClient);
	else
		m_pP2pClient = new CCoreClient(m_ProcessType);
	m_pP2pClient->SetEventListener(this);
	return CNetController::Get()->StartCoreClient(ip, port, m_pP2pClient);
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void	CP2PClient::Disconnect()
{
	if (m_pP2pClient)
		m_pP2pClient->Disconnect();
	if (m_pP2pHost)
		m_pP2pHost->Disconnect();
}


//------------------------------------------------------------------------
// p2pHost/Client 두 객체에게 Protocol Listener을 등록한다.
//------------------------------------------------------------------------
bool	CP2PClient::AddProtocolListener(ProtocolListenerPtr pListener)
{
	if (!CNetConnector::AddProtocolListener(pListener))
		return false;
	if (m_pP2pClient)
		m_pP2pClient->AddProtocolListener(pListener);
	if (m_pP2pHost)
		m_pP2pHost->AddProtocolListener(pListener);
	return true;
}


//------------------------------------------------------------------------
// p2pHost/Client 두 객체에게 적용한다.
//------------------------------------------------------------------------
bool	CP2PClient::RemoveProtocolListener(ProtocolListenerPtr pListener)
{
	if (!CNetConnector::RemoveProtocolListener(pListener))
		return false;
	if (m_pP2pClient)
		m_pP2pClient->RemoveProtocolListener(pListener);
	if (m_pP2pHost)
		m_pP2pHost->RemoveProtocolListener(pListener);
	return true;
}


/**
 @brief P2P Host Event Handler , P2P host Listen
 */
void	CP2PClient::OnListen(ServerBasicPtr svr)
{
	if (m_pEventListener)
		m_pEventListener->OnP2PCreate(this);
}


/**
 @brief P2P Host Event Handler , P2P disconnect
 */
void	CP2PClient::OnServerDisconnect(ServerBasicPtr svr)
{
	if (m_pEventListener)
		m_pEventListener->OnP2PDisconnect(this);
}


/**
 @brief P2P Host Event Handler , P2P Client join
 */
void	CP2PClient::OnClientJoin(ServerBasicPtr svr, netid netId)
{
	if (m_pEventListener)
		m_pEventListener->OnMemberJoin(this, netId);
}


/**
 @brief P2P Host Event Handler , P2P Client leave
 */
void	CP2PClient::OnClientLeave(ServerBasicPtr svr, netid netId)
{
	if (m_pEventListener)
		m_pEventListener->OnMemberLeave(this, netId);
}


/**
 @brief P2P Client Event Handler, P2P Client Connect
 */
void	CP2PClient::OnCoreClientConnect(CoreClientPtr client)
{
	if (m_pEventListener)
		m_pEventListener->OnP2PCreate(this);
}


/**
 @brief P2P Client Event Handler, P2P Client Disconnect
 */
void	CP2PClient::OnClientDisconnect(CoreClientPtr client)
{
	if (m_pEventListener)
		m_pEventListener->OnP2PDisconnect(this);
}

