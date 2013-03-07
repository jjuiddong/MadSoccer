
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
// ����� remoteClient�� ��� �����Ѵ�.
//------------------------------------------------------------------------
bool CServer::Stop()
{
	CNetController::Get()->StopServer(this);
	Disconnect();
	return true;
}


//------------------------------------------------------------------------
// Ŭ���̾�Ʈ �߰�
//------------------------------------------------------------------------
bool CServer::AddClient(SOCKET sock)
{
	EnterSync();
	{
 		RemoteClientItor it = FindRemoteClientBySocket(sock);
		if (m_RemoteClients.end() != it)
			return false; // �̹������Ѵٸ� ����

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
// ����Ʈ Ŭ���̾�Ʈ ���
//------------------------------------------------------------------------
CRemoteClient* CServer::GetRemoteClient(netid netId)
{
	RemoteClientItor it = m_RemoteClients.find(netId);
	if (m_RemoteClients.end() == it)
		return NULL; //���ٸ� ����
	return it->second;
}


//------------------------------------------------------------------------
// ����Ʈ Ŭ���̾�Ʈ ���
//------------------------------------------------------------------------
//CRemoteClient* CServer::GetRemoteClientFromSocket(SOCKET sock)
//{
//	RemoteClientItor it = FindRemoteClientBySocket(sock);
//	if (m_RemoteClients.end() == it)
//		return NULL; //���ٸ� ����
// 	return it->second;
//}


//------------------------------------------------------------------------
// ���Ϲ�ȣ�� netid �� ��´�.
//------------------------------------------------------------------------
netid CServer::GetNetIdFromSocket(SOCKET sock)
{
	RemoteClientItor it = FindRemoteClientBySocket(sock);
 	if (m_RemoteClients.end() == it)
 		return INVALID_NETID; //���ٸ� ����
 	return it->second->GetId();
}


//------------------------------------------------------------------------
// Ŭ���̾�Ʈ ����
//------------------------------------------------------------------------
bool CServer::RemoveClient(netid netId)
{
	EnterSync();
	{
		RemoteClientItor it = m_RemoteClients.find(netId);
		if (m_RemoteClients.end() == it)
			return false; //���ٸ� ����
		RemoveClientProcess(it);
	}
	LeaveSync();
	return true;
}


//------------------------------------------------------------------------
// Ŭ���̾�Ʈ ����
//------------------------------------------------------------------------
//bool CServer::RemoveClientBySocket(SOCKET sock)
//{
//	EnterSync();
//	{
//		RemoteClientItor it = FindRemoteClientBySocket(sock);
// 		if (m_RemoteClients.end() == it)
// 			return false; //���ٸ� ����
//  		RemoveClientProcess(it);
//	}
//	LeaveSync();
//	return true;
//}


//------------------------------------------------------------------------
// m_RemoteClients���� sock�� �ش��ϴ� Ŭ���̾�Ʈ�� �����Ѵ�.
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
// m_RemoteClients �����ȿ��� Client�� �����ؾ� �ɶ� ���̴� �Լ���.
// Client�� �����ϰ� ������ ����Ű�� iterator�� ��ȯ�Ѵ�.
//------------------------------------------------------------------------
RemoteClientItor CServer::RemoveClientInLoop(netid netId)
{
	RemoteClientItor it = m_RemoteClients.find(netId);
	if (m_RemoteClients.end() == it)
		return m_RemoteClients.end(); //���ٸ� ����

	RemoteClientItor r = RemoveClientProcess(it);
	return r;
}


//------------------------------------------------------------------------
// Ŭ���̾�Ʈ ���� ó��
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
// ��� ���� ����
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
// m_RemoteClients�� ����� socket���� fd_set�� �����Ѵ�. 
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
// �ش� socket�� sockets����Ʈ�� �����Ѵٸ� true�� �����Ѵ�.
//------------------------------------------------------------------------
bool CServer::IsExist(netid netId)
{
	RemoteClientItor it = m_RemoteClients.find(netId);
	return m_RemoteClients.end() != it;
}


//------------------------------------------------------------------------
// ����ȭ ����
//------------------------------------------------------------------------
void CServer::EnterSync()
{
	EnterCriticalSection( &m_CriticalSection );
}


//------------------------------------------------------------------------
// ����ȭ ��
//------------------------------------------------------------------------
void CServer::LeaveSync()
{
	LeaveCriticalSection( &m_CriticalSection );
}


//------------------------------------------------------------------------
// ��Ŷ ����
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
// ����� ��� Ŭ���̾�Ʈ���� �޼����� ������. 
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
// ����� ��� Ŭ���̾�Ʈ�� ������ ����, ���� ���ϵ� �ݴ´�.
//------------------------------------------------------------------------
void	CServer::Disconnect()
{
	// ���� �������� ���� 
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

