
#include "stdafx.h"
#include "Server.h"
#include <winsock.h>
#include <process.h> 
//#include <boost/bind.hpp>
#include "../Controller/NetController.h"

using namespace network;


CServer::CServer(PROCESS_TYPE procType) :
	m_IsServerOn(true)
,	m_ProcessType(procType)
,	m_RootGroup(NULL, "root")
{
	m_ServerPort = 2333;
	InitializeCriticalSection( &m_CriticalSection );

	CGroup *pWaitGroup = new CGroup(NULL,"Waiting Group");
	m_WaitGroupId = pWaitGroup->GetId();
	m_RootGroup.AddChild( pWaitGroup );
}

CServer::~CServer()
{
	Clear();
}


//------------------------------------------------------------------------
// 저장된 remoteClient를 모두 제거한다.
//------------------------------------------------------------------------
bool CServer::Stop()
{
	CNetController::Get()->StopServer(this);
	Disconnect();
	return true;
}


//------------------------------------------------------------------------
// 클라이언트 추가
//------------------------------------------------------------------------
bool CServer::AddClient(SOCKET sock)
{
	EnterSync();
	{
 		RemoteClientItor it = FindRemoteClientBySocket(sock);
		if (m_RemoteClients.end() != it)
			return false; // 이미존재한다면 실패

		CRemoteClient *pNewRemoteClient = new CRemoteClient();
		pNewRemoteClient->SetSocket(sock);

		pNewRemoteClient->SetGroupId(m_WaitGroupId);
		if (!m_RootGroup.AddUser(m_WaitGroupId, pNewRemoteClient->GetId()))
		{
			LogNPrint( "CServer::AddClient Error!! netid: %d", pNewRemoteClient->GetId());
			SAFE_DELETE(pNewRemoteClient);
		}
		else
		{
			m_RemoteClients.insert( 
				RemoteClientMap::value_type(pNewRemoteClient->GetId(), pNewRemoteClient) );

			LogNPrint( "AddClient netid: %d, socket: %d", pNewRemoteClient->GetId(), sock );
			OnClientJoin(pNewRemoteClient->GetId());
		}
	}
	LeaveSync();
	return true;
}


//------------------------------------------------------------------------
// 리모트 클라이언트 얻기
//------------------------------------------------------------------------
CRemoteClient* CServer::GetRemoteClient(netid netId)
{
	RemoteClientItor it = m_RemoteClients.find(netId);
	if (m_RemoteClients.end() == it)
		return NULL; //없다면 실패
	return it->second;
}


//------------------------------------------------------------------------
// 리모트 클라이언트 얻기
//------------------------------------------------------------------------
//CRemoteClient* CServer::GetRemoteClientFromSocket(SOCKET sock)
//{
//	RemoteClientItor it = FindRemoteClientBySocket(sock);
//	if (m_RemoteClients.end() == it)
//		return NULL; //없다면 실패
// 	return it->second;
//}


//------------------------------------------------------------------------
// 소켓번호로 netid 를 얻는다.
//------------------------------------------------------------------------
netid CServer::GetNetIdFromSocket(SOCKET sock)
{
	RemoteClientItor it = FindRemoteClientBySocket(sock);
 	if (m_RemoteClients.end() == it)
 		return INVALID_NETID; //없다면 실패
 	return it->second->GetId();
}


//------------------------------------------------------------------------
// 클라이언트 제거
//------------------------------------------------------------------------
bool CServer::RemoveClient(netid netId)
{
	EnterSync();
	{
		RemoteClientItor it = m_RemoteClients.find(netId);
		if (m_RemoteClients.end() == it)
			return false; //없다면 실패
		RemoveClientProcess(it);
	}
	LeaveSync();
	return true;
}


//------------------------------------------------------------------------
// 클라이언트 제거
//------------------------------------------------------------------------
//bool CServer::RemoveClientBySocket(SOCKET sock)
//{
//	EnterSync();
//	{
//		RemoteClientItor it = FindRemoteClientBySocket(sock);
// 		if (m_RemoteClients.end() == it)
// 			return false; //없다면 실패
//  		RemoveClientProcess(it);
//	}
//	LeaveSync();
//	return true;
//}


//------------------------------------------------------------------------
// m_RemoteClients에서 sock에 해당하는 클라이언트를 리턴한다.
//------------------------------------------------------------------------
RemoteClientItor CServer::FindRemoteClientBySocket(SOCKET sock)
{
	RemoteClientItor it = m_RemoteClients.begin();
	while (m_RemoteClients.end() != it)
	{
		if (it->second->GetSocket() == sock)
			return it;
		++it;
	}
	return m_RemoteClients.end();	
}


//------------------------------------------------------------------------
// m_RemoteClients 루프안에서 Client를 제거해야 될때 쓰이는 함수다.
// Client를 제거하고 다음을 가르키는 iterator를 반환한다.
//------------------------------------------------------------------------
RemoteClientItor CServer::RemoveClientInLoop(netid netId)
{
	RemoteClientItor it = m_RemoteClients.find(netId);
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
	const netid userId = it->second->GetId();
	if (!m_RootGroup.RemoveUser(it->second->GetGroupId(), userId))
	{
		LogNPrint( "CServer::RemoveClientProcess() Error!! not remove user groupid: %d, userid: %d",
			it->second->GetGroupId(), userId);
	}

	delete it->second;
	RemoteClientItor r = m_RemoteClients.erase(it);

	dbg::Print( "Leave Client %d", userId );
	OnClientLeave(userId);
	return r;
}


//------------------------------------------------------------------------
// 모든 소켓 제거
//------------------------------------------------------------------------
void CServer::Clear()
{
	m_IsServerOn = false;
	Sleep(100);

	BOOST_FOREACH( RemoteClientMap::value_type &kv, m_RemoteClients)
	{
		delete kv.second;
	}
	m_RemoteClients.clear();

	DeleteCriticalSection( &m_CriticalSection );
	closesocket(m_Socket);
	WSACleanup();
}


//------------------------------------------------------------------------
// m_RemoteClients에 저장된 socket으로 fd_set을 생성한다. 
//------------------------------------------------------------------------
void CServer::MakeFDSET( SFd_Set *pfdset)
{
	if (!pfdset)
		return;

	EnterSync();
	{
		FD_ZERO(pfdset);
		BOOST_FOREACH(RemoteClientMap::value_type &kv, m_RemoteClients)
		{
			//pfdset->fd_array[ pfdset->fd_count] = kv.second->GetSocket();
			//pfdset->fd_count++;
			FD_SET(kv.second->GetSocket(), (fd_set*)pfdset);
			pfdset->netid_array[ pfdset->fd_count-1] = kv.second->GetId();
		}
	}
	LeaveSync();
}


//------------------------------------------------------------------------
// 해당 socket이 sockets리스트에 존재한다면 true를 리턴한다.
//------------------------------------------------------------------------
bool CServer::IsExist(netid netId)
{
	RemoteClientItor it = m_RemoteClients.find(netId);
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
	RemoteClientItor it = m_RemoteClients.find(netId);
	if (m_RemoteClients.end() == it)
		return false;

	const int result = send(it->second->GetSocket(), packet.GetData(), packet.GetPacketSize(), 0);
	if (result == INVALID_SOCKET)
	{
		error::ErrorLog( common::format("CServer::Send() Socket Error id=%d", it->second->GetId()) );
		dbg::Print( "CServer::Send() Socket Error id=%d", it->second->GetId() );
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
		const int result = send(it->second->GetSocket(), packet.GetData(), packet.GetPacketSize(), 0);
		if (result == INVALID_SOCKET)
		{
			it = RemoveClientInLoop(it->second->GetId());
		}
		else
		{
			++it;
		}
	}

	return true;
}


//------------------------------------------------------------------------
// 연결된 모든 클라이언트와 연결을 끊고, 서버 소켓도 닫는다.
//------------------------------------------------------------------------
void	CServer::Disconnect()
{
	// 아직 구현하지 않음 
}


//------------------------------------------------------------------------
// Event Listen
//------------------------------------------------------------------------
void	CServer::OnListen()
{
	RET(!m_pEventListener);
	m_pEventListener->OnListen(this);
}

//------------------------------------------------------------------------
// Event Client Join
//------------------------------------------------------------------------
void	CServer::OnClientJoin(netid netId)
{
	RET(!m_pEventListener);
	m_pEventListener->OnClientJoin(this, netId);
}

//------------------------------------------------------------------------
// Event ClientLeave
//------------------------------------------------------------------------
void	CServer::OnClientLeave(netid netId)
{
	RET(!m_pEventListener);
	m_pEventListener->OnClientLeave(this, netId);
}

