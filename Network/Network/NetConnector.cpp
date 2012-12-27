
#include "stdafx.h"
#include "NetConnector.h"

using namespace network;


CNetConnector::CNetConnector() :
	m_NetId(common::GenerateId())
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
bool CNetConnector::AddListener(ProtocolListenerPtr pListener)
{
	ProtocolListenerMapItor it = m_ProtocolListeners.find(pListener->GetId());
	if (m_ProtocolListeners.end() == it)
	{
		m_ProtocolListeners.insert( ProtocolListenerMap::value_type(pListener->GetId(), ProtocolListenerList()) );
		m_ProtocolListeners[ pListener->GetId()].push_back(pListener);
	}
	else
	{
		it->second.push_back(pListener);
	}
	return true;
}


//------------------------------------------------------------------------
// ������ ���� (�޸𸮴� �ܺο��� �Ұ��ؾ��Ѵ�.)
//------------------------------------------------------------------------
bool CNetConnector::RemoveListener(ProtocolListenerPtr pListener)
{
	ProtocolListenerMapItor it = m_ProtocolListeners.find(pListener->GetId());
	if (m_ProtocolListeners.end() == it)
	{
		return false;
	}
	else
	{
		it->second.remove(pListener);
	}
	return true;
}


//------------------------------------------------------------------------
// ������ ���
//------------------------------------------------------------------------
const ProtocolListenerList&	CNetConnector::GetListeners(int listenerId)
{
	static ProtocolListenerList emptyList; // reference return �� �ӽ� ����
	ProtocolListenerMapItor it = m_ProtocolListeners.find(listenerId);
	if (m_ProtocolListeners.end() == it)
		return emptyList; // ������ �� ����Ʈ�� �����Ѵ�.
	return it->second;
}


//------------------------------------------------------------------------
// listenerId�� �ش��ϴ� �����ʰ� �����Ѵٸ� true�� �����Ѵ�.
//------------------------------------------------------------------------
bool CNetConnector::IsExistListener(int listenerId)
{
	ProtocolListenerMapItor it = m_ProtocolListeners.find(listenerId);
	return m_ProtocolListeners.end() != it;
}
