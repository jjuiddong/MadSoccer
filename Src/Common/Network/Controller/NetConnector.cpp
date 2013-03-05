
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
	ProtocolListenerItor it = find(m_ProtocolListeners.begin(), m_ProtocolListeners.end(), pListener);
	if (m_ProtocolListeners.end() != it)
		return false; // 이미 존재한다면 실패
	m_ProtocolListeners.push_back( pListener);
	return true;
}


//------------------------------------------------------------------------
// 리스너 제거 (메모리는 외부에서 소거해야한다.)
//------------------------------------------------------------------------
bool CNetConnector::RemoveListener(ProtocolListenerPtr pListener)
{
	ProtocolListenerItor it = find(m_ProtocolListeners.begin(), m_ProtocolListeners.end(), pListener);
	if (m_ProtocolListeners.end() == it)
		return false; // 없다면 실패
	m_ProtocolListeners.erase(it);
	return true;
}


//------------------------------------------------------------------------
// 커넥션 제거
//------------------------------------------------------------------------
void	CNetConnector::ClearConnection()
{
	closesocket(m_Socket);
	WSACleanup();
}
