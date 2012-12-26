
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
// 프로토콜 설정
//------------------------------------------------------------------------
void CNetObject::SetProtocol(ProtocolPtr protocol) 
{ 
	m_pProtocol = protocol; 
	m_pProtocol->SetNetObject(this);
}


//------------------------------------------------------------------------
// 리스너 등록
//------------------------------------------------------------------------
bool CNetObject::AddListener(IProtocolListener *pListener)
{
	ListenerItor it = std::find(m_Listners.begin(), m_Listners.end(), pListener);
	if (m_Listners.end() != it)
		return false; // 이미 존재한다면 실패

	m_Listners.push_back(pListener);
	return true;
}


//------------------------------------------------------------------------
// 리스너 제거
//------------------------------------------------------------------------
bool CNetObject::RemoveListener(IProtocolListener *listener)
{
	m_Listners.remove(listener);
	return true;
}

