
#include "stdafx.h"
#include "Client.h"
#include "../Controller/P2PClient.h"
#include "../Interface/Protocol.h"
#include "../Controller/NetController.h"
#include "AllProtocolListener.h"
#include "../Controller/CoreClient.h"

using namespace network;


CClient::CClient(PROCESS_TYPE procType) :
	m_ProcessType(procType)
,	m_pP2p(NULL)
,	m_pConnectSvr(NULL)
,	m_pEventListener(NULL)
{
	m_pConnectSvr = new CCoreClient(procType);
	m_pConnectSvr->SetEventListener(this);
}

CClient::~CClient() 
{
	Clear();
}


//------------------------------------------------------------------------
// Client 기능 멈춤
// 모든 소켓이 초기화 된다.
//------------------------------------------------------------------------
bool CClient::Stop()
{
	CNetController::Get()->StopClient(this);
	if (m_pConnectSvr)	
		m_pConnectSvr->Stop();
	if (m_pP2p)
		m_pP2p->Stop();
	return true;
}


//------------------------------------------------------------------------
// PROCESS_TYPE이 USER_LOOP일 때, 매 프레임마다 호출되어야 하는 함수다.
// 패킷이 서버로 부터 왔는지 검사한다.
//------------------------------------------------------------------------
bool CClient::Proc()
{
	if (m_pConnectSvr)	
		m_pConnectSvr->Proc();
	if (m_pP2p)
		m_pP2p->Proc();
	return true;
}


//------------------------------------------------------------------------
// 서버와 접속이 끊어질때 호출된다.
//------------------------------------------------------------------------
void CClient::Disconnect()
{
	if (m_pConnectSvr)
		m_pConnectSvr->Disconnect();
	if (m_pP2p)
		m_pP2p->Disconnect();

	//OnDisconnect();
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CClient::Clear()
{
	Stop();
	SAFE_DELETE(m_pConnectSvr);
	SAFE_DELETE(m_pP2p);
}


//------------------------------------------------------------------------
// P2p와 Server에 접속하는 coreClient에게도 Protocol을 설정한다.
//------------------------------------------------------------------------
bool	CClient::AddListener(ProtocolListenerPtr pListener)
{
	if (!CNetConnector::AddListener(pListener))
		return false;
	if (m_pConnectSvr)
		m_pConnectSvr->AddListener(pListener);
	if (m_pP2p)
		m_pP2p->AddListener(pListener);
	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool	CClient::RemoveListener(ProtocolListenerPtr pListener)
{
	if (!CNetConnector::RemoveListener(pListener))
		return false;
	if (m_pConnectSvr)
		m_pConnectSvr->RemoveListener(pListener);
	if (m_pP2p)
		m_pP2p->RemoveListener(pListener);
	return true;
}


//------------------------------------------------------------------------
// 패킷 전송
//------------------------------------------------------------------------
bool CClient::Send(netid netId, const CPacket &packet)
{
	if (P2P_NETID == netId)
	{
		if (m_pP2p)
			m_pP2p->Send(netId, packet);
	}
	else
	{
		if (m_pConnectSvr)
			m_pConnectSvr->Send(netId, packet);
	}
	return true;
}


//------------------------------------------------------------------------
// 연결된 모든 클라이언트들에게 메세지를 보낸다.
//------------------------------------------------------------------------
bool CClient::SendAll(const CPacket &packet)
{
	// 아직 아무것도 없음
	return true;
}


//------------------------------------------------------------------------
// 서버와 연결되어 있다면 true를 리턴한다.
//------------------------------------------------------------------------
bool	CClient::IsConnect() const 
{
	return m_pConnectSvr && m_pConnectSvr->IsConnect();
}


//------------------------------------------------------------------------
// Connect Event Handler
//------------------------------------------------------------------------
void	CClient::OnCoreClientConnect(CoreClientPtr client)
{
	RET(!m_pEventListener);
	m_pEventListener->OnClientConnect(this);
}


//------------------------------------------------------------------------
// Disconnect Event Handler
//------------------------------------------------------------------------
void	CClient::OnClientDisconnect(CoreClientPtr client)
{
	RET(!m_pEventListener);
	m_pEventListener->OnClientDisconnect(this);
}

