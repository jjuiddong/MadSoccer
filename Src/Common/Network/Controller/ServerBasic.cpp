#include "stdafx.h"
#include "ServerBasic.h"
#include <winsock.h>
#include <process.h> 
#include "../Controller/NetController.h"
#include "../ProtocolHandler/BasicProtocolDispatcher.h"

using namespace network;


CServerBasic::CServerBasic(PROCESS_TYPE procType) :
	CNetConnector(procType)
,	m_RootGroup(NULL, "root")
,	m_pSessionFactory(new CSessionFactory()) // defalut 
,	m_pGroupFactory(new CGroupFactory()) // default
{
	SetPort(2333);
	m_Timers.reserve(10);

	InitRootGroup();
}

CServerBasic::~CServerBasic()
{
	Clear();
	m_Timers.clear();
	SAFE_DELETE(m_pSessionFactory);
	SAFE_DELETE(m_pGroupFactory);
}


/**
 @brief SetSessionFactory
 */
void	CServerBasic::SetSessionFactory( ISessionFactory *ptr ) 
{ 
	SAFE_DELETE(m_pSessionFactory);
	m_pSessionFactory = ptr; 
}


/**
 @brief SetGroupFactory
 */
void	 CServerBasic::SetGroupFactory( IGroupFactory *ptr )
{
	SAFE_DELETE(m_pGroupFactory);
	m_pGroupFactory = ptr;
}


/**
 @brief 
 */
void	CServerBasic::InitRootGroup()
{
	m_RootGroup.Clear();

	CGroup *pWaitGroup = m_pGroupFactory->New(); //new CGroup(NULL,"Waiting Group");
	pWaitGroup->SetName("Waiting Group");
	pWaitGroup->AddViewer(m_RootGroup.GetId()); /// root group is viewer to waitting group
	m_WaitGroupId = pWaitGroup->GetId();
	m_RootGroup.AddChild( pWaitGroup );
}


/**
 @brief packet process
*/
void	CServerBasic::Proc()
{
	/// Accept
	AcceptProcess();
	
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
			const netid senderId = sockets.netid_array[ i];
			if (result == SOCKET_ERROR || result == 0) // ���� ��Ŷ����� 0�̸� ������ ����ٴ� �ǹ̴�.
			{
				RemoveSessionSocket(sockets.netid_array[ i]);

				CPacketQueue::Get()->PushPacket( 
					CPacketQueue::SPacketData(GetNetId(), 
						ClientDisconnectPacket(senderId, CNetController::Get()->GetUniqueValue()) ));
			}
			else
			{
				CPacketQueue::Get()->PushPacket( 
					CPacketQueue::SPacketData(GetNetId(), CPacket(senderId, buf)) );
			}
		}
	}

	/// Dispatch Packet
	DispatchPacket();

}


/**
 @brief Accept
 */
bool CServerBasic::AcceptProcess()
{
	const timeval t = {0, 10}; // 10 millisecond
	fd_set readSockets;
	FD_ZERO(&readSockets);
	FD_SET(GetSocket(), &readSockets);

	const int ret = select( readSockets.fd_count, &readSockets, NULL, NULL, &t );
	if (ret != 0 && ret != SOCKET_ERROR)
	{
		for (u_int i=0; i < readSockets.fd_count; ++i)
		{
			// accept(��û�� ���� ����, ���� Ŭ���̾�Ʈ �ּ�)
			SOCKET remoteSocket = accept(readSockets.fd_array[ i], NULL, NULL);
			if (remoteSocket == INVALID_SOCKET)
			{
				clog::Error( clog::ERROR_CRITICAL, "Client�� Accept�ϴ� ���߿� ������ �߻���\n" );
				return true;
			}

			// get ip address
			sockaddr_in addr;
			int len = sizeof(addr);
			memset(&addr,0,sizeof(addr));
			getpeername( remoteSocket, (sockaddr*)&addr, &len );
			std::string ip = inet_ntoa(addr.sin_addr);

			CPacketQueue::Get()->PushPacket( 
				CPacketQueue::SPacketData(GetNetId(), AcceptPacket(remoteSocket, ip)) );
			//AddSession( remoteSocket, ip );
		}
	}
	return true; 
}


/**
 @brief Dispatch Packet
 */
void	CServerBasic::DispatchPacket()
{
	CPacketQueue::SPacketData packetData;
	if (!CPacketQueue::Get()->PopPacket(GetNetId(), packetData))
		return;

	const ProtocolListenerList &listeners = GetProtocolListeners();

	// ��� ��Ŷ�� �޾Ƽ� ó���ϴ� �����ʿ��� ��Ŷ�� ������.
	all::Dispatcher allDispatcher;
	allDispatcher.Dispatch(packetData.packet, listeners);
	// 

	const int protocolId = packetData.packet.GetProtocolId();

	// �⺻ �������� ó��
	if (protocolId == 0)
	{
		basic_protocol::ServerDispatcher dispatcher;
		dispatcher.Dispatch( packetData.packet, this );
		return;
	}

	if (listeners.empty())
	{
		clog::Error(clog::ERROR_CRITICAL, " CServerBasic::DispatchPacket() �������� �����ʰ� �����ϴ�. netid: %d\n", GetNetId() );
		return;
	}

	IProtocolDispatcher *pDispatcher = CNetController::Get()->GetDispatcher(protocolId);
	if (!pDispatcher)
	{
		clog::Error( clog::ERROR_WARNING,
			common::format(" CServerBasic::DispatchPacket() %d �� �ش��ϴ� �������� �����İ� �����ϴ�.\n", 
			protocolId) );
	}
	else
	{
		pDispatcher->Dispatch(packetData.packet, listeners);
	}
}


//------------------------------------------------------------------------
// ����� Session �� ��� �����Ѵ�.
//------------------------------------------------------------------------
bool CServerBasic::Stop()
{
	CNetController::Get()->StopServer(this);
	return true;
}


//------------------------------------------------------------------------
// Ŭ���̾�Ʈ �߰�
//------------------------------------------------------------------------
bool CServerBasic::AddSession(SOCKET sock, const std::string &ip)
{
	RETV(!m_pSessionFactory, false);

	common::AutoCSLock cs(m_CS);

	SessionItor it = FindSessionBySocket(sock);
	if (m_Sessions.end() != it)
		return false; // �̹������Ѵٸ� ����

	CSession *pNewSession = m_pSessionFactory->New();
	pNewSession->SetSocket(sock);
	pNewSession->SetIp(ip);
	pNewSession->SetState(SESSIONSTATE_LOGIN_WAIT);

	if (!m_RootGroup.AddUser(m_WaitGroupId, pNewSession->GetNetId()))
	{
		clog::Error( clog::ERROR_CRITICAL, "CServerBasic::AddClient Error!! netid: %d\n", pNewSession->GetNetId());
		SAFE_DELETE(pNewSession);
	}
	else
	{
		m_Sessions.insert( 
			Sessions_::value_type(pNewSession->GetNetId(), pNewSession) );

		clog::Log( clog::LOG_F_N_O, "AddClient netid: %d, socket: %d\n", pNewSession->GetNetId(), sock );
		OnClientJoin(pNewSession->GetNetId());
	}

	return true;
}


//------------------------------------------------------------------------
// ����Ʈ Ŭ���̾�Ʈ ���
//------------------------------------------------------------------------
CSession* CServerBasic::GetSession(netid netId)
{
	SessionItor it = m_Sessions.find(netId);
	if (m_Sessions.end() == it)
		return NULL; //���ٸ� ����
	return it->second;
}


/**
 @brief  ����Ʈ Ŭ���̾�Ʈ ���
 */
CSession* CServerBasic::GetSession(const std::string &clientId)
{
	BOOST_FOREACH(auto &client, m_Sessions.m_Seq)
	{
		if (client && client->GetName() == clientId)
		{
			return client;
		}
	}
	return NULL;
}


//------------------------------------------------------------------------
// ���Ϲ�ȣ�� netid �� ��´�.
//------------------------------------------------------------------------
netid CServerBasic::GetNetIdFromSocket(SOCKET sock)
{
	SessionItor it = FindSessionBySocket(sock);
	if (m_Sessions.end() == it)
		return INVALID_NETID; //���ٸ� ����
	return it->second->GetNetId();
}


//------------------------------------------------------------------------
// Ŭ���̾�Ʈ ����
//------------------------------------------------------------------------
bool CServerBasic::RemoveSession(netid netId)
{
	common::AutoCSLock cs(m_CS);

	SessionItor it = m_Sessions.find(netId);
	if (m_Sessions.end() == it)
		return false; //���ٸ� ����
	RemoveClientProcess(it);
	return true;
}


/**
 @brief socket �� �����Ѵ�. ������ �߻��� socket�� ���� �����ϰ�,
 ������ ������ �������� �����Ѵ�.
 ������ �߻��� �������� ��Ŷ�� ��� �޴� ���� ���� ���ؼ�
 */
bool	CServerBasic::RemoveSessionSocket(netid netId)
{
	CSession *pClient = GetSession(netId);
	if (!pClient)
		return false;

	closesocket(pClient->GetSocket());
	pClient->SetSocket(0);
	return true;
}


//------------------------------------------------------------------------
// m_Sessions���� sock�� �ش��ϴ� Ŭ���̾�Ʈ�� �����Ѵ�.
//------------------------------------------------------------------------
SessionItor CServerBasic::FindSessionBySocket(SOCKET sock)
{
	SessionItor it = m_Sessions.begin();
	while (m_Sessions.end() != it)
	{
		if (it->second->GetSocket() == sock)
			return it;
		++it;
	}
	return m_Sessions.end();	
}


//------------------------------------------------------------------------
// m_Sessions �����ȿ��� Client�� �����ؾ� �ɶ� ���̴� �Լ���.
// Client�� �����ϰ� ������ ����Ű�� iterator�� ��ȯ�Ѵ�.
//------------------------------------------------------------------------
//SessionItor CServerBasic::RemoveSessionInLoop(netid netId)
//{
//	SessionItor it = m_RemoteClients.find(netId);
//	if (m_RemoteClients.end() == it)
//		return m_RemoteClients.end(); //���ٸ� ����
//
//	SessionItor r = RemoveClientProcess(it);
//	return r;
//}


//------------------------------------------------------------------------
// Ŭ���̾�Ʈ ���� ó��
//------------------------------------------------------------------------
bool CServerBasic::RemoveClientProcess(SessionItor it)
{
	const netid userId = it->second->GetNetId();
	const SOCKET sock = it->second->GetSocket();

	// call before remove client
	OnClientLeave(userId);

	 GroupPtr pGroup = m_RootGroup.GetChildFromUser(userId);
	 if (pGroup)
	 {
		if (!m_RootGroup.RemoveUser(pGroup->GetId(), userId))
		{
			clog::Error( clog::ERROR_PROBLEM, "CServer::RemoveClientProcess() Error!! not remove user groupid: %d, userid: %d\n",
				pGroup->GetId(), userId);
		}
	 }
	 else
	 {
		 clog::Error( clog::ERROR_PROBLEM, "CServer::RemoveClientProcess() Error!! not found group userid: %d\n",userId);
	 }

	delete it->second;
	m_Sessions.remove(userId);

	clog::Log( clog::LOG_F_N_O, "RemoveClient netid: %d, socket: %d\n", userId, sock );
	return true;
}


//------------------------------------------------------------------------
// ��� ���� ����
//------------------------------------------------------------------------
void CServerBasic::Clear()
{
	SetState(SESSIONSTATE_DISCONNECT);
	BOOST_FOREACH( auto &kv, m_Sessions.m_Seq)
	{
		delete kv;
	}
	m_Sessions.clear();

	m_RootGroup.Clear();

	ClearConnection();
}


//------------------------------------------------------------------------
// m_Sessions�� ����� socket���� fd_set�� �����Ѵ�. 
//------------------------------------------------------------------------
void CServerBasic::MakeFDSET( SFd_Set *pfdset)
{
	if (!pfdset)
		return;

	common::AutoCSLock cs(m_CS);

	FD_ZERO(pfdset);
	BOOST_FOREACH(auto &kv, m_Sessions.m_Seq)
	{
		//pfdset->fd_array[ pfdset->fd_count] = kv.second->GetSocket();
		//pfdset->fd_count++;
		FD_SET(kv->GetSocket(), (fd_set*)pfdset);
		pfdset->netid_array[ pfdset->fd_count-1] = kv->GetNetId();
	}
}


//------------------------------------------------------------------------
// �ش� socket�� sockets����Ʈ�� �����Ѵٸ� true�� �����Ѵ�.
//------------------------------------------------------------------------
bool CServerBasic::IsExist(netid netId)
{
	SessionItor it = m_Sessions.find(netId);
	return m_Sessions.end() != it;
}


//------------------------------------------------------------------------
// ����� ��� Ŭ���̾�Ʈ���� �޼����� ������. 
//------------------------------------------------------------------------
bool CServerBasic::SendAll(const CPacket &packet)
{
	BOOST_FOREACH(auto &client, m_Sessions.m_Seq)
	{
		if (!client)
			continue;

		const int result = send(client->GetSocket(), packet.GetData(), CPacket::MAX_PACKETSIZE, 0);
		if (result == INVALID_SOCKET)
		{
			Send(GetNetId(), SEND_T, 
				ClientDisconnectPacket(CNetController::Get()->GetUniqueValue(), client->GetNetId()) );
		}
	}

	return true;
}


/**
 @brief 
 */
bool	CServerBasic::Send(netid netId, const SEND_FLAG flag, const CPacket &packet)
{
	bool sendResult = true;
	if ((flag == SEND_T) || (flag == SEND_T_V))
	{
		SessionItor it = m_Sessions.find(netId);
		if (m_Sessions.end() != it) // Send To Client
		{
			const int result = send(it->second->GetSocket(), packet.GetData(), CPacket::MAX_PACKETSIZE, 0);
			if (result == INVALID_SOCKET)
			{
				clog::Error( clog::ERROR_WARNING, common::format("CServer::Send() Socket Error id=%d\n", 
					it->second->GetNetId()) );
				RemoveSession(packet.GetSenderId());
				sendResult = false;
			}
		}
		else
		{
			// Send To Group
			GroupPtr pGroup = (m_RootGroup.GetId() == netId)? &m_RootGroup : m_RootGroup.GetChild(netId);
			if (pGroup)
			{
				const bool result = SendGroup(pGroup, packet);
				if (!result)
					sendResult = false;
			}
			else if (GetNetId() == netId) // ���� �ڽſ��� ������ ��Ŷ
			{
				CPacketQueue::Get()->PushPacket( CPacketQueue::SPacketData(GetNetId(), packet) );
			}
			else
			{
				sendResult = false;
			}
		}
	}

	if ((flag == SEND_V) || (flag == SEND_T_V))
	{
		const bool result = SendViewer(netId, flag, packet);
		if (!result)
			sendResult = false;
	}

	return sendResult;
}


/**
 @brief groupId�� ���� ��Ŷ�� �����Ѵ�.
 @param groupId: group netid �� ���� �����Ѵ�.
 @param flag: SEND_VIEWER, SEND_TARGET_VIEWER Ÿ���� �� �����ϴ� �Լ���.
 */
bool	CServerBasic::SendViewer(netid groupId, const SEND_FLAG flag, const CPacket &packet)
{
	if ((flag != SEND_V) && (flag != SEND_T_V))
		return false;

	GroupPtr pGroup = (m_RootGroup.GetId() == groupId)? &m_RootGroup : m_RootGroup.GetChild(groupId);
	if (!pGroup)
		return false;

	bool sendResult = true;
	const bool IsGroupSend = (flag == SEND_T) || (flag == SEND_T_V);

	BOOST_FOREACH(auto &viewerId, pGroup->GetViewers())
	{
		const bool result = SendViewerRecursive(viewerId, ((IsGroupSend)? pGroup->GetId() : INVALID_NETID), packet);
		if (!result)
			sendResult = result;
	}

	return sendResult;
}


/**
 @brief pGroup �� �Ҽӵ� user�鿡�� ��Ŷ�� �����Ѵ�.
 @param excetpGroupId ���� �ش��ϴ� group�� �����ϰ� ��Ŷ�� �����Ѵ�.
 */
bool	CServerBasic::SendViewerRecursive(netid viewerId, const netid exceptGroupId, const CPacket &packet)
{
	if (exceptGroupId == viewerId)
		return true;

	GroupPtr pGroup = (m_RootGroup.GetId() == viewerId)? &m_RootGroup : m_RootGroup.GetChild(viewerId);
	if (!pGroup)
		return false;

	if (exceptGroupId == INVALID_NETID)
	{
		return SendGroup(pGroup, packet);
	}
	else
	{
		if( pGroup->GetChildren().empty())
		{
			return SendGroup(pGroup, packet);
		}
		else
		{
			bool sendResult = true;
			BOOST_FOREACH(auto &child, pGroup->GetChildren())
			{
				if (!child) continue;
				const bool result = SendViewerRecursive(child->GetId(), exceptGroupId, packet);
				if (!result)
					sendResult = false;
			}
			return sendResult;
		}
	}
}


/**
 @brief 
 */
bool	CServerBasic::SendGroup(GroupPtr pGroup, const CPacket &packet)
{
	RETV(!pGroup, false);
	BOOST_FOREACH(auto &userId, pGroup->GetUsers())
	{
		Send(userId, SEND_TARGET, packet);
	}
	return true;
}


//------------------------------------------------------------------------
// ���� ������ TaskWork���� ������.
//------------------------------------------------------------------------
void	CServerBasic::Disconnect()
{
	Close();
	CNetController::Get()->RemoveServer(this);
	Clear();
	InitRootGroup();
}


/**
@brief  close socket
*/
void	CServerBasic::Close()
{
	SetState(SESSIONSTATE_DISCONNECT);
	ClearConnection();
}


//------------------------------------------------------------------------
// Event Listen
//------------------------------------------------------------------------
void	CServerBasic::OnListen()
{
	SetState(SESSIONSTATE_LOGIN);
	SearchEventTable( CNetEvent(EVT_LISTEN, this) );
}


/**
 @brief Bind ���н� ȣ��ȴ�.
 */
void	CServerBasic::OnDisconnect()
{
	SearchEventTable( CNetEvent(EVT_DISCONNECT, this) );
}

//------------------------------------------------------------------------
// Event Client Join
//------------------------------------------------------------------------
void	CServerBasic::OnClientJoin(netid netId)
{
	SearchEventTable( CNetEvent(EVT_CLIENT_JOIN, this, netId) );
}

//------------------------------------------------------------------------
// Event ClientLeave
//------------------------------------------------------------------------
void	CServerBasic::OnClientLeave(netid netId)
{
	SearchEventTable( CNetEvent(EVT_CLIENT_LEAVE, this, netId) );
}


/**
 @brief Timer
 */
void	CServerBasic::OnTimer(int id)
{
	SearchEventTable( CEvent(EVT_TIMER, id) );
}


/**
 @brief Ÿ�̸� �߰�
 */
void	CServerBasic::AddTimer( int id, int intervalTime, bool isRepeat ) //  isRepeat = true;
{
	STimer timer(id, intervalTime, GetTickCount(), isRepeat);
	auto it = std::find(m_Timers.begin(), m_Timers.end(), timer);
	if (m_Timers.end() == it)
	{
		m_Timers.push_back( timer );
	}
	else
	{
		it->interval = intervalTime;
	}
}


/**
 @brief Ÿ�̸� ����
 */
void	CServerBasic::KillTimer( int id )
{
	common::removevector(m_Timers, STimer(id) );
}


/**
 @brief 
 */
void	CServerBasic::MainLoop()
{

	//------------------------------------------------------------
	// Timer ó�� 
	if (m_Timers.empty())
		return;
	const int curT = GetTickCount();
	for (u_int i=0; i < m_Timers.size(); ++i)
	{
		if ((curT - m_Timers[ i].beginT) > m_Timers[ i].interval)
		{
			const int id = m_Timers[ i].id;
			if (!m_Timers[ i].repeat)
			{
				KillTimer( id );
				OnTimer( id );
			}
			else
			{
				OnTimer( id );
				m_Timers[ i].beginT = curT;
			}
			break; // OnTimer�� �ѹ��� ȣ���ϰ� �����Ѵ�. OnTimer ȣ�� ���� killtimer �� ȣ���ϸ� ������ ���� �� �ֱ� ����.
		}
	}	
	//------------------------------------------------------------

}
