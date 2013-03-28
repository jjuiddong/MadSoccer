
#include "stdafx.h"
#include "NetConnector.h"

using namespace network;


CNetConnector::CNetConnector(PROCESS_TYPE procType) :
	m_Id(common::GenerateId())
,	m_ProcessType(procType)
,	m_pParent(NULL)
,	m_hThread(NULL)
{

}

CNetConnector::~CNetConnector()
{

}


//------------------------------------------------------------------------
// �������� �߰�
//------------------------------------------------------------------------
bool CNetConnector::RegisterProtocol(ProtocolPtr protocol)
{
	protocol->SetNetConnector(this);
	return true;
}


//------------------------------------------------------------------------
// ������ ���
//------------------------------------------------------------------------
bool CNetConnector::AddProtocolListener(ProtocolListenerPtr pListener)
{
	ProtocolListenerItor it = find(m_ProtocolListeners.begin(), m_ProtocolListeners.end(), pListener);
	if (m_ProtocolListeners.end() != it)
		return false; // �̹� �����Ѵٸ� ����
	m_ProtocolListeners.push_back( pListener);
	return true;
}


//------------------------------------------------------------------------
// ������ ���� (�޸𸮴� �ܺο��� �Ұ��ؾ��Ѵ�.)
//------------------------------------------------------------------------
bool CNetConnector::RemoveProtocolListener(ProtocolListenerPtr pListener)
{
	ProtocolListenerItor it = find(m_ProtocolListeners.begin(), m_ProtocolListeners.end(), pListener);
	if (m_ProtocolListeners.end() == it)
		return false; // ���ٸ� ����
	m_ProtocolListeners.erase(it);
	return true;
}


//------------------------------------------------------------------------
// Ŀ�ؼ� ����
//------------------------------------------------------------------------
void	CNetConnector::ClearConnection()
{
	closesocket(m_Socket);
	m_Socket = 0;
	//WSACleanup();
}
