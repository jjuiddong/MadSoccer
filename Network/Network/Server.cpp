
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
		return false; //�̹� �����Ѵٸ� ����

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
		return false; //���ٸ� ����

	m_ClientSockets.remove(sock);
	OnClientLeave(sock);
	return true;
}


//------------------------------------------------------------------------
// m_ClientSockets �����ȿ��� Client�� �����ؾ� �ɶ� ���̴� �Լ���.
// Client�� �����ϰ� ������ ����Ű�� iterator�� ��ȯ�Ѵ�.
//------------------------------------------------------------------------
SockItor CServer::RemoveClientInLoop(SOCKET sock)
{
	SockItor it = find(m_ClientSockets.begin(), m_ClientSockets.end(), sock );
	if (m_ClientSockets.end() == it)
		return m_ClientSockets.end(); //���ٸ� ����

	SockItor re = m_ClientSockets.erase(it);
	OnClientLeave(sock);
	return re;
}


//------------------------------------------------------------------------
// ��� ���� ����
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
// m_ClientSockets�� ����� socket���� fd_set�� �����Ѵ�. 
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
// �ش� socket�� sockets����Ʈ�� �����Ѵٸ� true�� �����Ѵ�.
//------------------------------------------------------------------------
bool CServer::IsExist(SOCKET socket)
{
	SockItor it = find(m_ClientSockets.begin(), m_ClientSockets.end(), socket);
	return m_ClientSockets.end() != it;
}


//------------------------------------------------------------------------
// ��Ŷ ����
//------------------------------------------------------------------------
bool CServer::Send(SOCKET sock, const CPacket &packet)
{
	// send(����� ����, ���� ����, ������ ����, ���°�)
	const int result = send(sock, packet.GetData(), packet.GetPacketSize(), 0);
	if (result == INVALID_SOCKET)
	{
		RemoveClient(packet.GetSenderSocket());
		return false;
	}
	return true;
}


//------------------------------------------------------------------------
// ����� ��� Ŭ���̾�Ʈ���� �޼����� ������. 
//------------------------------------------------------------------------
bool CServer::SendAll(const CPacket &packet)
{
	SockItor it = m_ClientSockets.begin();
	while (m_ClientSockets.end() != it)
	{
		// send(����� ����, ���� ����, ������ ����, ���°�)
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
// ������ ���
//------------------------------------------------------------------------
bool CServer::AddListener(IPacketListener *pListener)
{
	ListenerItor it = std::find(m_Listners.begin(), m_Listners.end(), pListener);
	if (m_Listners.end() != it)
		return false; // �̹� �����Ѵٸ� ����

	m_Listners.push_back(pListener);
	return true;
}


//------------------------------------------------------------------------
// ������ ����
//------------------------------------------------------------------------
bool CServer::RemoveListener(IPacketListener *listener)
{
	m_Listners.remove(listener);
	return true;
}

