
#include "stdafx.h"
#include "Plug.h"

using namespace network;


CPlug::CPlug(PROCESS_TYPE procType) :
	m_ProcessType(procType)
,	m_pParent(NULL)
,	m_hThread(NULL)
{

}

CPlug::~CPlug()
{

}


//------------------------------------------------------------------------
// �������� �߰�
//------------------------------------------------------------------------
bool CPlug::RegisterProtocol(ProtocolPtr protocol)
{
	protocol->SetNetConnector(this);
	return true;
}


//------------------------------------------------------------------------
// ������ ���
//------------------------------------------------------------------------
bool CPlug::AddProtocolListener(ProtocolListenerPtr pListener)
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
bool CPlug::RemoveProtocolListener(ProtocolListenerPtr pListener)
{
	ProtocolListenerItor it = find(m_ProtocolListeners.begin(), m_ProtocolListeners.end(), pListener);
	if (m_ProtocolListeners.end() == it)
		return false; // ���ٸ� ����
	m_ProtocolListeners.erase(it);
	return true;
}
