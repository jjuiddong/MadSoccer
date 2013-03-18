#include "stdafx.h"
#include "ServerBasic.h"
#include <winsock.h>
#include <process.h> 
#include "../Controller/NetController.h"

using namespace network;


CServerBasic::CServerBasic(PROCESS_TYPE procType) :
m_IsServerOn(true)
	,	m_ProcessType(procType)
	,	m_RootGroup(NULL, "root")
	,	m_hThread(NULL)
{
	m_ServerPort = 2333;
	InitializeCriticalSection( &m_CriticalSection );
}

CServerBasic::~CServerBasic()
{
	Clear();
}


/**
 @brief packet process
*/
void	CServerBasic::Proc()
{

	const timeval t = {0, 10}; // 10 millisecond
	SFd_Set readSockets;
	MakeFDSET(&readSockets);
	const SFd_Set sockets = readSockets;

	const int ret = select( readSockets.fd_count, &readSockets, NULL, NULL, &t);
	if (ret != 0 && ret != SOCKET_ERROR)
	{
		for (u_int i=0; i < sockets.fd_count; ++i)
		{
			if (!FD_ISSET(sockets.fd_array[ i], &readSockets)) continue;

			char buf[ CPacket::MAX_PACKETSIZE];
			const int result = recv(sockets.fd_array[ i], buf, sizeof(buf), 0);
			if (result == INVALID_SOCKET || 0 == result)
			{
				RemoveRemoteClient(sockets.netid_array[ i]);
			}
			else
			{
				const ProtocolListenerList &listeners = GetProtocolListeners();
				if (listeners.empty())
				{
					error::ErrorLog( " CServerBasic::Proc():: �������� �����ʰ� �����ϴ�.");
				}
				else
				{
					CPacket packet(SERVER_NETID,buf);

					// ��� ��Ŷ�� �޾Ƽ� ó���ϴ� �����ʿ��� ��Ŷ�� ������.
					all::Dispatcher allDispatcher;
					allDispatcher.Dispatch(packet, listeners);
					// 

					const int protocolId = packet.GetProtocolId();
					IProtocolDispatcher *pDispatcher = CNetController::Get()->GetDispatcher(protocolId);
					if (!pDispatcher)
					{
						error::ErrorLog( 
							common::format(" CClientCore::Proc() %d �� �ش��ϴ� �������� �����İ� �����ϴ�.", 
							protocolId) );
					}
					else
					{
						pDispatcher->Dispatch(packet, listeners);
					}
				}

			}
		}
	}

}


//------------------------------------------------------------------------
// ����� remoteClient�� ��� �����Ѵ�.
//------------------------------------------------------------------------
bool CServerBasic::Stop()
{
	CNetController::Get()->StopServer(this);
	Disconnect();
	return true;
}


//------------------------------------------------------------------------
// Ŭ���̾�Ʈ �߰�
//------------------------------------------------------------------------
bool CServerBasic::AddRemoteClient(SOCKET sock, const std::string &ip)
{
	EnterSync();
	{
		RemoteClientItor it = FindRemoteClientBySocket(sock);
		if (m_RemoteClients.end() != it)
			return false; // �̹������Ѵٸ� ����

		CRemoteClient *pNewRemoteClient = new CRemoteClient();
		pNewRemoteClient->SetSocket(sock);
		pNewRemoteClient->SetIp(ip);
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
CRemoteClient* CServerBasic::GetRemoteClient(netid netId)
{
	RemoteClientItor it = m_RemoteClients.find(netId);
	if (m_RemoteClients.end() == it)
		return NULL; //���ٸ� ����
	return it->second;
}


//------------------------------------------------------------------------
// ���Ϲ�ȣ�� netid �� ��´�.
//------------------------------------------------------------------------
netid CServerBasic::GetNetIdFromSocket(SOCKET sock)
{
	RemoteClientItor it = FindRemoteClientBySocket(sock);
	if (m_RemoteClients.end() == it)
		return INVALID_NETID; //���ٸ� ����
	return it->second->GetId();
}


//------------------------------------------------------------------------
// Ŭ���̾�Ʈ ����
//------------------------------------------------------------------------
bool CServerBasic::RemoveRemoteClient(netid netId)
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
// m_RemoteClients���� sock�� �ش��ϴ� Ŭ���̾�Ʈ�� �����Ѵ�.
//------------------------------------------------------------------------
RemoteClientItor CServerBasic::FindRemoteClientBySocket(SOCKET sock)
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
RemoteClientItor CServerBasic::RemoveRemoteClientInLoop(netid netId)
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
RemoteClientItor CServerBasic::RemoveClientProcess(RemoteClientItor it)
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
void CServerBasic::Clear()
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
void CServerBasic::MakeFDSET( SFd_Set *pfdset)
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
bool CServerBasic::IsExist(netid netId)
{
	RemoteClientItor it = m_RemoteClients.find(netId);
	return m_RemoteClients.end() != it;
}


//------------------------------------------------------------------------
// ����ȭ ����
//------------------------------------------------------------------------
void CServerBasic::EnterSync()
{
	EnterCriticalSection( &m_CriticalSection );
}


//------------------------------------------------------------------------
// ����ȭ ��
//------------------------------------------------------------------------
void CServerBasic::LeaveSync()
{
	LeaveCriticalSection( &m_CriticalSection );
}


//------------------------------------------------------------------------
// ��Ŷ ����
//------------------------------------------------------------------------
bool CServerBasic::Send(netid netId, const CPacket &packet)
{
	RemoteClientItor it = m_RemoteClients.find(netId);
	if (m_RemoteClients.end() == it)
		return false;

	const int result = send(it->second->GetSocket(), packet.GetData(), packet.GetPacketSize(), 0);
	if (result == INVALID_SOCKET)
	{
		error::ErrorLog( common::format("CServer::Send() Socket Error id=%d", it->second->GetId()) );
		dbg::Print( "CServer::Send() Socket Error id=%d", it->second->GetId() );
		RemoveRemoteClient(packet.GetSenderId());
		return false;
	}
	return true;
}


//------------------------------------------------------------------------
// ����� ��� Ŭ���̾�Ʈ���� �޼����� ������. 
//------------------------------------------------------------------------
bool CServerBasic::SendAll(const CPacket &packet)
{
	RemoteClientItor it = m_RemoteClients.begin();
	while (m_RemoteClients.end() != it)
	{
		const int result = send(it->second->GetSocket(), packet.GetData(), packet.GetPacketSize(), 0);
		if (result == INVALID_SOCKET)
		{
			it = RemoveRemoteClientInLoop(it->second->GetId());
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
void	CServerBasic::Disconnect()
{
	// ���� �������� ���� 
}


//------------------------------------------------------------------------
// Event Listen
//------------------------------------------------------------------------
void	CServerBasic::OnListen()
{
	RET(!m_pEventListener);
	m_pEventListener->OnListen(this);
}

//------------------------------------------------------------------------
// Event Client Join
//------------------------------------------------------------------------
void	CServerBasic::OnClientJoin(netid netId)
{
	RET(!m_pEventListener);
	m_pEventListener->OnClientJoin(this, netId);
}

//------------------------------------------------------------------------
// Event ClientLeave
//------------------------------------------------------------------------
void	CServerBasic::OnClientLeave(netid netId)
{
	RET(!m_pEventListener);
	m_pEventListener->OnClientLeave(this, netId);
}
