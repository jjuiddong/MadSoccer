
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
{
	m_pConnectSvr = new CCoreClient(procType);

}


CClient::~CClient() 
{
	Clear();

}


//------------------------------------------------------------------------
// Client ��� ����
// ��� ������ �ʱ�ȭ �ȴ�.
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
// �� �����Ӹ��� ȣ��Ǿ�� �ϴ� �Լ���.
// ��Ŷ�� ������ ���� �Դ��� �˻��Ѵ�.
//------------------------------------------------------------------------
bool CClient::Proc()
{
	return true;
}


//------------------------------------------------------------------------
// ������ ������ �������� ȣ��ȴ�.
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
// ��Ŷ ����
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
// ����� ��� Ŭ���̾�Ʈ�鿡�� �޼����� ������.
//------------------------------------------------------------------------
bool CClient::SendAll(const CPacket &packet)
{
	// ���� �ƹ��͵� ����
	return true;
}


//------------------------------------------------------------------------
// ������ ����Ǿ� �ִٸ� true�� �����Ѵ�.
//------------------------------------------------------------------------
bool	CClient::IsConnect() const 
{
	return m_pConnectSvr && m_pConnectSvr->IsConnect();
}

