
#include "stdafx.h"
#include "P2PClient.h"
#include "CoreClient.h"
#include "NetController.h"

using namespace network;

CP2PClient::CP2PClient(PROCESS_TYPE procType) :
	m_ProcessType(procType)
,	m_pP2pClient(NULL)
,	m_pP2pHost(NULL)
{

}

CP2PClient::~CP2PClient()
{
	Clear();
}


//------------------------------------------------------------------------
// P2P 통신을 시도한다.
// state: Host = P2P Host Server 상태가 되고, port 번호로 서버를 실행한다.
//		       Client = P2P Client 상태가 되고, ip, port 로 접속을 시도한다.
//------------------------------------------------------------------------
bool	CP2PClient::Connect( P2P_STATE state, const int port, const std::string &ip ) // ip=""
{
	m_State = state;

	bool result = false;
	switch (state)
	{
	case P2P_HOST: result = CreateP2PHost(port); break;
	case P2P_CLIENT: result = CreateP2PClient(ip,port); break;
	}
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
		m_pP2pHost = new CServer(m_ProcessType);
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
bool	CP2PClient::AddListener(ProtocolListenerPtr pListener)
{
	if (!CNetConnector::AddListener(pListener))
		return false;
	if (m_pP2pClient)
		m_pP2pClient->AddListener(pListener);
	if (m_pP2pHost)
		m_pP2pHost->AddListener(pListener);
	return true;
}


//------------------------------------------------------------------------
// p2pHost/Client 두 객체에게 적용한다.
//------------------------------------------------------------------------
bool	CP2PClient::RemoveListener(ProtocolListenerPtr pListener)
{
	if (!CNetConnector::RemoveListener(pListener))
		return false;
	if (m_pP2pClient)
		m_pP2pClient->RemoveListener(pListener);
	if (m_pP2pHost)
		m_pP2pHost->RemoveListener(pListener);
	return true;
}


//------------------------------------------------------------------------
// Event MemberJoin
//------------------------------------------------------------------------
void	CP2PClient::OnMemberJoin()
{
	//RET(!m_pEventListener);
	//m_pEventListener->OnMemberJoin(this);
}


//------------------------------------------------------------------------
// Event MemberLeave
//------------------------------------------------------------------------
void	CP2PClient::OnMemberLeave()
{
	//RET(!m_pEventListener);
	//m_pEventListener->OnMemberLeave(this);
}

