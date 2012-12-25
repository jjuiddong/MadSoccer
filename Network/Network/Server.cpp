
#include "stdafx.h"
#include "Server.h"
#include <winsock.h>
#include <process.h> 


using namespace network;


CServer::CServer() :
	m_Id(common::GenerateId())
,	m_IsServerOn(true)
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
// 
//------------------------------------------------------------------------
bool CServer::AddClient(SOCKET sock)
{
	SockItor it = find(m_ClientSockets.begin(), m_ClientSockets.end(), sock );
	if (m_ClientSockets.end() != it)
		return false; //이미 존재한다면 실패

	m_ClientSockets.push_back( sock );
	OnClientJoin(sock);
	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CServer::RemoveClient(SOCKET sock)
{
	SockItor it = find(m_ClientSockets.begin(), m_ClientSockets.end(), sock );
	if (m_ClientSockets.end() == it)
		return false; //없다면 실패

	m_ClientSockets.remove(sock);
	OnClientLeave(sock);
	return true;
}


//------------------------------------------------------------------------
// m_ClientSockets 루프안에서 Client를 제거해야 될때 쓰이는 함수다.
// Client를 제거하고 다음을 가르키는 iterator를 반환한다.
//------------------------------------------------------------------------
SockItor CServer::RemoveClientInLoop(SOCKET sock)
{
	SockItor it = find(m_ClientSockets.begin(), m_ClientSockets.end(), sock );
	if (m_ClientSockets.end() == it)
		return m_ClientSockets.end(); //없다면 실패

	SockItor re = m_ClientSockets.erase(it);
	OnClientLeave(sock);
	return re;
}


//------------------------------------------------------------------------
// 모든 소켓 제거
//------------------------------------------------------------------------
void CServer::Clear()
{
	m_IsServerOn = false;
	Sleep(100);

	SockItor it = m_ClientSockets.begin();
	while (m_ClientSockets.end() != it)
	{
		SOCKET sock = *it++;
		closesocket(sock);
	}
	m_ClientSockets.clear();

	DeleteCriticalSection( &m_CriticalSection );
	closesocket(m_Socket);
	WSACleanup();
}


//------------------------------------------------------------------------
// m_ClientSockets에 저장된 socket으로 fd_set을 생성한다. 
//------------------------------------------------------------------------
void CServer::MakeFDSET( fd_set *pfdset)
{
	if (!pfdset)
		return;

	FD_ZERO(pfdset);
	SockItor it = m_ClientSockets.begin();
	while (m_ClientSockets.end() != it)
	{
		pfdset->fd_array[ pfdset->fd_count] = *it++;
		pfdset->fd_count++;
	}
}


//------------------------------------------------------------------------
// 해당 socket이 sockets리스트에 존재한다면 true를 리턴한다.
//------------------------------------------------------------------------
bool CServer::IsExist(SOCKET socket)
{
	SockItor it = find(m_ClientSockets.begin(), m_ClientSockets.end(), socket);
	return m_ClientSockets.end() != it;
}


//------------------------------------------------------------------------
// 패킷 전송
//------------------------------------------------------------------------
bool CServer::Send(SOCKET sock, const CPacket &packet)
{
	// send(연결된 소켓, 보낼 버퍼, 버퍼의 길이, 상태값)
	const int result = send(sock, packet.GetData(), packet.GetPacketSize(), 0);
	if (result == INVALID_SOCKET)
	{
		RemoveClient(packet.GetSenderSocket());
		return false;
	}
	return true;
}


//------------------------------------------------------------------------
// 연결된 모든 클라이언트에게 메세지를 보낸다. 
//------------------------------------------------------------------------
bool CServer::SendAll(const CPacket &packet)
{
	SockItor it = m_ClientSockets.begin();
	while (m_ClientSockets.end() != it)
	{
		// send(연결된 소켓, 보낼 버퍼, 버퍼의 길이, 상태값)
		const int result = send(*it, packet.GetData(), packet.GetPacketSize(), 0);
		if (result == INVALID_SOCKET)
		{
			it = RemoveClientInLoop(*it);
		}
		else
		{
			++it;
		}
	}

	return true;
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
// 리스너 등록
//------------------------------------------------------------------------
bool CServer::AddListener(IPacketListener *pListener)
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
bool CServer::RemoveListener(IPacketListener *listener)
{
	m_Listners.remove(listener);
	return true;
}

