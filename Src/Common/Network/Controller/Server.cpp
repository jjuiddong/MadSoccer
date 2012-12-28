
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
	RemoteClientItor it = find_if(m_RemoteClients.begin(), m_RemoteClients.end(), 
		bind(IsSameSocket<CRemoteClient>,_1,sock) );
	if (m_RemoteClients.end() != it)
		return false; // �̹������Ѵٸ� ����

	CRemoteClient *pNewRemoteClient = new CRemoteClient();
	pNewRemoteClient->SetSocket(sock);
	m_RemoteClients.push_back( pNewRemoteClient );

	error::Log( common::format("AddClient netid: %d", pNewRemoteClient->GetNetId()) );

	OnClientJoin(sock);
	return true;
}


//------------------------------------------------------------------------
// ����Ʈ Ŭ���̾�Ʈ ���
//------------------------------------------------------------------------
CRemoteClient* CServer::GetRemoteClient(netid netId)
{
	RemoteClientItor it = find_if(m_RemoteClients.begin(), m_RemoteClients.end(), 
		bind(IsSameNetId<CRemoteClient>,_1,netId));
	if (m_RemoteClients.end() == it)
		return NULL; //���ٸ� ����
	return *it;
}


//------------------------------------------------------------------------
// ����Ʈ Ŭ���̾�Ʈ ���
//------------------------------------------------------------------------
CRemoteClient* CServer::GetRemoteClientFromSocket(SOCKET sock)
{
	RemoteClientItor it = find_if(m_RemoteClients.begin(), m_RemoteClients.end(), 
		bind(IsSameSocket<CRemoteClient>,_1,sock));
	if (m_RemoteClients.end() == it)
		return NULL; //���ٸ� ����
	return *it;
}


//------------------------------------------------------------------------
// ���Ϲ�ȣ�� netid �� ��´�.
//------------------------------------------------------------------------
netid CServer::GetNetIdFromSocket(SOCKET sock)
{
	RemoteClientItor it = find_if(m_RemoteClients.begin(), m_RemoteClients.end(), 
		bind(IsSameSocket<CRemoteClient>,_1,sock));
	if (m_RemoteClients.end() == it)
		return INVALID_NETID; //���ٸ� ����
	return (*it)->GetNetId();
}


//------------------------------------------------------------------------
// Ŭ���̾�Ʈ ����
//------------------------------------------------------------------------
bool CServer::RemoveClient(netid netId)
{
	RemoteClientItor it = find_if(m_RemoteClients.begin(), m_RemoteClients.end(), 
		bind(IsSameNetId<CRemoteClient>,_1,netId));
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
	RemoteClientItor it = find_if(m_RemoteClients.begin(), m_RemoteClients.end(), 
		bind(IsSameSocket<CRemoteClient>,_1,sock));
	if (m_RemoteClients.end() == it)
		return false; //���ٸ� ����

	RemoveClientProcess(it);
	return true;
}


//------------------------------------------------------------------------
// m_RemoteClients �����ȿ��� Client�� �����ؾ� �ɶ� ���̴� �Լ���.
// Client�� �����ϰ� ������ ����Ű�� iterator�� ��ȯ�Ѵ�.
//------------------------------------------------------------------------
RemoteClientItor CServer::RemoveClientInLoop(netid netId)
{
	RemoteClientItor it = find_if(m_RemoteClients.begin(), m_RemoteClients.end(), 
		bind(IsSameNetId<CRemoteClient>,_1,netId));
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
	const netid netId = (*it)->GetNetId();
	delete *it;
	RemoteClientItor r = m_RemoteClients.erase(it);

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
// m_RemoteClients�� ����� socket���� fd_set�� �����Ѵ�. 
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
// �ش� socket�� sockets����Ʈ�� �����Ѵٸ� true�� �����Ѵ�.
//------------------------------------------------------------------------
bool CServer::IsExist(netid netId)
{
	RemoteClientItor it = find_if(m_RemoteClients.begin(), m_RemoteClients.end(), 
		bind(IsSameNetId<CRemoteClient>,_1,netId) );
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
	RemoteClientItor it = find_if(m_RemoteClients.begin(), m_RemoteClients.end(), 
		bind(IsSameNetId<CRemoteClient>,_1,netId));
	if (m_RemoteClients.end() == it)
		return false;

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
// ����� ��� Ŭ���̾�Ʈ���� �޼����� ������. 
//------------------------------------------------------------------------
bool CServer::SendAll(const CPacket &packet)
{
	RemoteClientItor it = m_RemoteClients.begin();
	while (m_RemoteClients.end() != it)
	{
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
