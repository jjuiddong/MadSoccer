
#include "stdafx.h"
#include "Server.h"
#include <winsock.h>
#include <process.h> 
#include <boost/bind.hpp>


using namespace network;


CServer::CServer() :
	m_IsServerOn(true)
{
	m_ServerPort = 2333;
	InitializeCriticalSection( &m_CriticalSection );

}


CServer::~CServer()
{
	Clear();
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CServer::Stop()
{

	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CServer::OnListen()
{

}


//------------------------------------------------------------------------
// 클라이언트 추가
//------------------------------------------------------------------------
bool CServer::AddClient(SOCKET sock)
{
	RemoteClientItor it = find_if(m_RemoteClients.begin(), m_RemoteClients.end(), 
		bind(IsSameSocket<CRemoteClient>,_1,sock) );
	if (m_RemoteClients.end() != it)
		return false; // 이미존재한다면 실패

	CRemoteClient *pNewRemoteClient = new CRemoteClient();
	pNewRemoteClient->SetSocket(sock);
	m_RemoteClients.push_back( pNewRemoteClient );

	error::Log( common::format("AddClient netid: %d", pNewRemoteClient->GetNetId()) );

	OnClientJoin(sock);
	return true;
}


//------------------------------------------------------------------------
// 리모트 클라이언트 얻기
//------------------------------------------------------------------------
CRemoteClient* CServer::GetRemoteClient(netid netId)
{
	RemoteClientItor it = find_if(m_RemoteClients.begin(), m_RemoteClients.end(), 
		bind(IsSameNetId<CRemoteClient>,_1,netId));
	if (m_RemoteClients.end() == it)
		return NULL; //없다면 실패
	return *it;
}


//------------------------------------------------------------------------
// 리모트 클라이언트 얻기
//------------------------------------------------------------------------
CRemoteClient* CServer::GetRemoteClientFromSocket(SOCKET sock)
{
	RemoteClientItor it = find_if(m_RemoteClients.begin(), m_RemoteClients.end(), 
		bind(IsSameSocket<CRemoteClient>,_1,sock));
	if (m_RemoteClients.end() == it)
		return NULL; //없다면 실패
	return *it;
}


//------------------------------------------------------------------------
// 소켓번호로 netid 를 얻는다.
//------------------------------------------------------------------------
netid CServer::GetNetIdFromSocket(SOCKET sock)
{
	RemoteClientItor it = find_if(m_RemoteClients.begin(), m_RemoteClients.end(), 
		bind(IsSameSocket<CRemoteClient>,_1,sock));
	if (m_RemoteClients.end() == it)
		return INVALID_NETID; //없다면 실패
	return (*it)->GetNetId();
}


//------------------------------------------------------------------------
// 클라이언트 제거
//------------------------------------------------------------------------
bool CServer::RemoveClient(netid netId)
{
	RemoteClientItor it = find_if(m_RemoteClients.begin(), m_RemoteClients.end(), 
		bind(IsSameNetId<CRemoteClient>,_1,netId));
	if (m_RemoteClients.end() == it)
		return false; //없다면 실패

	RemoveClientProcess(it);
	return true;
}


//------------------------------------------------------------------------
// 클라이언트 제거
//------------------------------------------------------------------------
bool CServer::RemoveClientFromSocket(SOCKET sock)
{
	RemoteClientItor it = find_if(m_RemoteClients.begin(), m_RemoteClients.end(), 
		bind(IsSameSocket<CRemoteClient>,_1,sock));
	if (m_RemoteClients.end() == it)
		return false; //없다면 실패

	RemoveClientProcess(it);
	return true;
}


//------------------------------------------------------------------------
// m_RemoteClients 루프안에서 Client를 제거해야 될때 쓰이는 함수다.
// Client를 제거하고 다음을 가르키는 iterator를 반환한다.
//------------------------------------------------------------------------
RemoteClientItor CServer::RemoveClientInLoop(netid netId)
{
	RemoteClientItor it = find_if(m_RemoteClients.begin(), m_RemoteClients.end(), 
		bind(IsSameNetId<CRemoteClient>,_1,netId));
	if (m_RemoteClients.end() == it)
		return m_RemoteClients.end(); //없다면 실패

	RemoteClientItor r = RemoveClientProcess(it);
	return r;
}


//------------------------------------------------------------------------
// 클라이언트 제거 처리
//------------------------------------------------------------------------
RemoteClientItor CServer::RemoveClientProcess(RemoteClientItor it)
{
	const netid netId = (*it)->GetNetId();
	delete *it;
	RemoteClientItor r = m_RemoteClients.erase(it);

	OnClientLeave(netId);
	return r;
}


//------------------------------------------------------------------------
// 모든 소켓 제거
//------------------------------------------------------------------------
void CServer::Clear()
{
	m_IsServerOn = false;
	Sleep(100);

	BOOST_FOREACH(CRemoteClient *p, m_RemoteClients)
	{
		delete p;
	}
	m_RemoteClients.clear();

	DeleteCriticalSection( &m_CriticalSection );
	closesocket(m_Socket);
	WSACleanup();
}


//------------------------------------------------------------------------
// m_RemoteClients에 저장된 socket으로 fd_set을 생성한다. 
//------------------------------------------------------------------------
void CServer::MakeFDSET( fd_set *pfdset)
{
	if (!pfdset)
		return;

	FD_ZERO(pfdset);
	BOOST_FOREACH(CRemoteClient *p, m_RemoteClients)
	{
		pfdset->fd_array[ pfdset->fd_count] = p->GetSocket();
		pfdset->fd_count++;
	}
}


//------------------------------------------------------------------------
// 해당 socket이 sockets리스트에 존재한다면 true를 리턴한다.
//------------------------------------------------------------------------
bool CServer::IsExist(netid netId)
{
	RemoteClientItor it = find_if(m_RemoteClients.begin(), m_RemoteClients.end(), 
		bind(IsSameNetId<CRemoteClient>,_1,netId) );
	return m_RemoteClients.end() != it;
}


//------------------------------------------------------------------------
// 동기화 시작
//------------------------------------------------------------------------
void CServer::EnterSync()
{
	EnterCriticalSection( &m_CriticalSection );
}


//------------------------------------------------------------------------
// 동기화 끝
//------------------------------------------------------------------------
void CServer::LeaveSync()
{
	LeaveCriticalSection( &m_CriticalSection );
}


//------------------------------------------------------------------------
// 패킷 전송
//------------------------------------------------------------------------
bool CServer::Send(netid netId, const CPacket &packet)
{
	RemoteClientItor it = find_if(m_RemoteClients.begin(), m_RemoteClients.end(), bind(IsSameNetId<CRemoteClient>,_1,netId));
	if (m_RemoteClients.end() == it)
		return false;

	// send(연결된 소켓, 보낼 버퍼, 버퍼의 길이, 상태값)
	const int result = send((*it)->GetSocket(), packet.GetData(), packet.GetPacketSize(), 0);
	if (result == INVALID_SOCKET)
	{
		error::ErrorLog( common::format("CServer::Send() Socket Error id=%d", (*it)->GetNetId()) );
		RemoveClient(packet.GetSenderId());
		return false;
	}
	return true;
}


//------------------------------------------------------------------------
// 연결된 모든 클라이언트에게 메세지를 보낸다. 
//------------------------------------------------------------------------
bool CServer::SendAll(const CPacket &packet)
{
	RemoteClientItor it = m_RemoteClients.begin();
	while (m_RemoteClients.end() != it)
	{
		// send(연결된 소켓, 보낼 버퍼, 버퍼의 길이, 상태값)
		const int result = send((*it)->GetSocket(), packet.GetData(), packet.GetPacketSize(), 0);
		if (result == INVALID_SOCKET)
		{
			it = RemoveClientInLoop((*it)->GetNetId());
		}
		else
		{
			++it;
		}
	}

	return true;
}
