
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
// 프로토콜 추가
//------------------------------------------------------------------------
bool CPlug::RegisterProtocol(ProtocolPtr protocol)
{
	protocol->SetNetConnector(this);
	return true;
}


//------------------------------------------------------------------------
// 리스너 등록
//------------------------------------------------------------------------
bool CPlug::AddProtocolListener(ProtocolListenerPtr pListener)
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
bool CPlug::RemoveProtocolListener(ProtocolListenerPtr pListener)
{
	ProtocolListenerItor it = find(m_ProtocolListeners.begin(), m_ProtocolListeners.end(), pListener);
	if (m_ProtocolListeners.end() == it)
		return false; // 없다면 실패
	m_ProtocolListeners.erase(it);
	return true;
}
