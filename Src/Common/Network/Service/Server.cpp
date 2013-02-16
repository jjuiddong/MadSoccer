
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
// Ŭ���̾�Ʈ �߰�
//------------------------------------------------------------------------
bool CServer::AddClient(SOCKET sock)
{
 	RemoteClientItor it = FindRemoteClientBySocket(sock);
	if (m_RemoteClients.end() != it)
		return false; // �̹������Ѵٸ� ����

	CRemoteClient *pNewRemoteClient = new CRemoteClient();
	pNewRemoteClient->SetSocket(sock);
	m_RemoteClients.insert( 
		RemoteClientMap::value_type(pNewRemoteClient->GetNetId(), pNewRemoteClient) );

	clog::Log( "AddClient netid: %d, socket: %d", pNewRemoteClient->GetNetId(), sock );
	dbg::Print( "AddClient netid: %d, socket: %d", pNewRemoteClient->GetNetId(), sock);

	OnClientJoin(pNewRemoteClient->GetNetId());
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
CRemoteClient* CServer::GetRemoteClientFromSocket(SOCKET sock)
{
	RemoteClientItor it = FindRemoteClientBySocket(sock);
	if (m_RemoteClients.end() == it)
		return NULL; //���ٸ� ����
 	return it->second;
}


//------------------------------------------------------------------------
// ���Ϲ�ȣ�� netid �� ��´�.
//------------------------------------------------------------------------
netid CServer::GetNetIdFromSocket(SOCKET sock)
{
	RemoteClientItor it = FindRemoteClientBySocket(sock);
 	if (m_RemoteClients.end() == it)
 		return INVALID_NETID; //���ٸ� ����
 	return it->second->GetNetId();
}


//------------------------------------------------------------------------
// Ŭ���̾�Ʈ ����
//------------------------------------------------------------------------
bool CServer::RemoveClient(netid netId)
{
	RemoteClientItor it = m_RemoteClients.find(netId);
	if (m_RemoteClients.end() == it)
		return false; //���ٸ� ����

	RemoveClientProcess(it);
	return true;
}


//------------------------------------------------------------------------
// Ŭ���̾�Ʈ ����
//------------------------------------------------------------------------
bool CServer::RemoveClientBySocket(SOCKET sock)
{
	RemoteClientItor it = FindRemoteClientBySocket(sock);
 	if (m_RemoteClients.end() == it)
 		return false; //���ٸ� ����
 
 	RemoveClientProcess(it);
	return true;
}


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
	const netid netId = it->second->GetNetId();
	delete it->second;
	RemoteClientItor r = m_RemoteClients.erase(it);

	dbg::Print( "Leave Client %d", netId );
	OnClientLeave(netId);
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
void CServer::MakeFDSET( fd_set *pfdset)
{
	if (!pfdset)
		return;

	FD_ZERO(pfdset);
	BOOST_FOREACH(RemoteClientMap::value_type &kv, m_RemoteClients)
	{
		pfdset->fd_array[ pfdset->fd_count] = kv.second->GetSocket();
		pfdset->fd_count++;
	}
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
		error::ErrorLog( common::format("CServer::Send() Socket Error id=%d", it->second->GetNetId()) );
		dbg::Print( "CServer::Send() Socket Error id=%d", it->second->GetNetId() );
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
			it = RemoveClientInLoop(it->second->GetNetId());
		}
		else
		{
			++it;
		}
	}

	return true;
}
