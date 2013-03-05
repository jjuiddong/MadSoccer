
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
bool	CP2PClient::Connect( STATE state, const int port, const std::string &ip ) // ip=""
{
	m_State = state;

	bool result = false;
	switch (state)
	{
	case Host: result = CreateP2PHost(port); break;
	case Client: result = CreateP2PClient(ip,port); break;
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
