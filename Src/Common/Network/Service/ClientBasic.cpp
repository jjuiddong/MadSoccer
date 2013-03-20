#include "stdafx.h"
#include "ClientBasic.h"
#include "../Controller/P2PClient.h"
#include "../Interface/Protocol.h"
#include "../Controller/NetController.h"
#include "AllProtocolListener.h"
#include "../Controller/CoreClient.h"

using namespace network;


CClientBasic::CClientBasic(PROCESS_TYPE procType) :
m_ProcessType(procType)
	,	m_pP2p(NULL)
	,	m_pConnectSvr(NULL)
	,	m_pEventListener(NULL)
{
	m_pConnectSvr = new CCoreClient(procType);
	m_pConnectSvr->SetEventListener(this);
	m_pP2p = new CP2PClient(procType);
	m_pP2p->SetEventListener(this);

}

CClientBasic::~CClientBasic() 
{
	Clear();
}


//------------------------------------------------------------------------
// Client ��� ����
// ��� ������ �ʱ�ȭ �ȴ�.
//------------------------------------------------------------------------
bool CClientBasic::Stop()
{
	CNetController::Get()->StopClient(this);
	if (m_pConnectSvr)	
		m_pConnectSvr->Stop();
	if (m_pP2p)
		m_pP2p->Stop();
	return true;
}


//------------------------------------------------------------------------
// PROCESS_TYPE�� USER_LOOP�� ��, �� �����Ӹ��� ȣ��Ǿ�� �ϴ� �Լ���.
// ��Ŷ�� ������ ���� �Դ��� �˻��Ѵ�.
//------------------------------------------------------------------------
bool CClientBasic::Proc()
{
	if (m_pConnectSvr)	
		m_pConnectSvr->Proc();
	if (m_pP2p)
		m_pP2p->Proc();
	return true;
}


//------------------------------------------------------------------------
// ������ ������ �������� ȣ��ȴ�.
//------------------------------------------------------------------------
void CClientBasic::Disconnect()
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
void CClientBasic::Clear()
{
	Stop();
	SAFE_DELETE(m_pConnectSvr);
	SAFE_DELETE(m_pP2p);
}


//------------------------------------------------------------------------
// P2p�� Server�� �����ϴ� coreClient���Ե� Protocol�� �����Ѵ�.
//------------------------------------------------------------------------
bool	CClientBasic::AddProtocolListener(ProtocolListenerPtr pListener)
{
	if (!CNetConnector::AddProtocolListener(pListener))
		return false;
	if (m_pConnectSvr)
		m_pConnectSvr->AddProtocolListener(pListener);
	if (m_pP2p)
		m_pP2p->AddProtocolListener(pListener);
	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool	CClientBasic::RemoveProtocolListener(ProtocolListenerPtr pListener)
{
	if (!CNetConnector::RemoveProtocolListener(pListener))
		return false;
	if (m_pConnectSvr)
		m_pConnectSvr->RemoveProtocolListener(pListener);
	if (m_pP2p)
		m_pP2p->RemoveProtocolListener(pListener);
	return true;
}


/**
 @brief Send Packet
 */
bool	CClientBasic::Send(netid netId, const SEND_FLAG flag, const CPacket &packet)
{
	if (P2P_NETID == netId)
	{
		return SendP2P(packet);
	}
	else
	{
		if (!m_pConnectSvr)
			return false;
		return m_pConnectSvr->Send(netId, flag, packet);
	}
}


/**
 @brief Send P2P Packet
 */
bool	CClientBasic::SendP2P(const CPacket &packet)
{
	if (!m_pP2p)
		return false;
	return m_pP2p->Send(P2P_NETID, SEND_TARGET, packet);
}


//------------------------------------------------------------------------
// ����� ��� Ŭ���̾�Ʈ�鿡�� �޼����� ������.
//------------------------------------------------------------------------
bool CClientBasic::SendAll(const CPacket &packet)
{
	// ���� �ƹ��͵� ����
	return true;
}


//------------------------------------------------------------------------
// ������ ����Ǿ� �ִٸ� true�� �����Ѵ�.
//------------------------------------------------------------------------
bool	CClientBasic::IsConnect() const 
{
	return m_pConnectSvr && m_pConnectSvr->IsConnect();
}


//------------------------------------------------------------------------
// Connect Event Handler
//------------------------------------------------------------------------
void	CClientBasic::OnCoreClientConnect(CoreClientPtr client)
{
	RET(!m_pEventListener);
	m_pEventListener->OnClientConnect(this);
}


//------------------------------------------------------------------------
// Disconnect Event Handler
//------------------------------------------------------------------------
void	CClientBasic::OnClientDisconnect(CoreClientPtr client)
{
	RET(!m_pEventListener);
	m_pEventListener->OnClientDisconnect(this);
}


/**
 @brief P2P Network Create, when server binding, client connect
 */
void	CClientBasic::OnP2PCreate(P2PClientPtr client)
{
	if (m_pEventListener)
		m_pEventListener->OnP2PConnect(this);
}


/**
 @brief P2P join client
 */
void	CClientBasic::OnMemberJoin(P2PClientPtr client, netid clientId)
{
	if (m_pEventListener)
		m_pEventListener->OnMemberJoin(this, clientId);
}


/**
 @brief P2P leave client
 */
void	CClientBasic::OnMemberLeave(P2PClientPtr client, netid clientId)
{
	if (m_pEventListener)
		m_pEventListener->OnMemberLeave(this, clientId);
}

