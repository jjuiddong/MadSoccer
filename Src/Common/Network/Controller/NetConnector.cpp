
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
// 프로토콜 추가
//------------------------------------------------------------------------
bool CNetConnector::RegisterProtocol(ProtocolPtr protocol)
{
	protocol->SetNetConnector(this);
	return true;
}


//------------------------------------------------------------------------
// 리스너 등록
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
// 리스너 제거 (메모리는 외부에서 소거해야한다.)
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
// 리스너 얻기
//------------------------------------------------------------------------
const ProtocolListenerList&	CNetConnector::GetListeners(int listenerId)
{
	static ProtocolListenerList emptyList; // reference return 용 임시 변수
	ProtocolListenerMapItor it = m_ProtocolListeners.find(listenerId);
	if (m_ProtocolListeners.end() == it)
		return emptyList; // 없으면 빈 리스트를 리턴한다.
	return it->second;
}


//------------------------------------------------------------------------
// listenerId에 해당하는 리스너가 존재한다면 true를 리턴한다.
//------------------------------------------------------------------------
bool CNetConnector::IsExistListener(int listenerId)
{
	ProtocolListenerMapItor it = m_ProtocolListeners.find(listenerId);
	return m_ProtocolListeners.end() != it;
}
