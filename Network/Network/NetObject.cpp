
#include "stdafx.h"
#include "NetObject.h"

using namespace network;


CNetObject::CNetObject() :
	m_NetId(common::GenerateId())
{

}

CNetObject::~CNetObject()
{

}


//------------------------------------------------------------------------
// �������� ����
//------------------------------------------------------------------------
void CNetObject::SetProtocol(ProtocolPtr protocol) 
{ 
	m_pProtocol = protocol; 
	m_pProtocol->SetNetObject(this);
}


//------------------------------------------------------------------------
// ������ ���
//------------------------------------------------------------------------
bool CNetObject::AddListener(IProtocolListener *pListener)
{
	ListenerItor it = std::find(m_Listners.begin(), m_Listners.end(), pListener);
	if (m_Listners.end() != it)
		return false; // �̹� �����Ѵٸ� ����

	m_Listners.push_back(pListener);
	return true;
}


//------------------------------------------------------------------------
// ������ ����
//------------------------------------------------------------------------
bool CNetObject::RemoveListener(IProtocolListener *listener)
{
	m_Listners.remove(listener);
	return true;
}

