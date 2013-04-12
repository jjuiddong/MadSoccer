
#include "stdafx.h"
#include "NetController.h"
#include "NetLauncher.h"
#include "ServerBasic.h"
#include "../Service/Server.h"
#include "../Service/Client.h"
#include "CoreClient.h"
#include "../Task/TaskLogic.h"
#include "../task/TaskAccept.h"
#include "../task/TaskWorkClient.h"
#include "../task/TaskWorkServer.h"
#include "../task/TaskWork.h"
#include <boost/bind.hpp>


using namespace network;

CNetController::CNetController() :
	m_AcceptThread("AcceptThread")
,	m_Servers(VECTOR_RESERVED_SIZE)
,	m_Clients(VECTOR_RESERVED_SIZE)
,	m_pSeperateServerWorkThread(NULL)
,	m_pSeperateClientWorkThread(NULL)
{

}

CNetController::~CNetController() 
{
	Clear();
}


//------------------------------------------------------------------------
// logicThreadCount ������ŭ ���������带 �����Ѵ�.
//------------------------------------------------------------------------
bool CNetController::Init(int logicThreadCount)
{
	// ���������� ����
	for (int i=0; i < logicThreadCount; ++i)
	{
		common::CThread *pThread = new common::CThread("LogicThread");
		pThread->AddTask( new CTaskLogic() );
		pThread->Start();
		m_LogicThreads.push_back( pThread );
	}

	// Accept ������ ����
	if (logicThreadCount > 0)
	{
  		m_AcceptThread.AddTask( new CTaskAccept() );
  		m_AcceptThread.Start();
	}

	m_UniqueValue = rand();

	return true;
}


/**
@brief  User Thread Packet process
*/
void CNetController::Proc()
{
	// client user loop
	BOOST_FOREACH(ClientBasicPtr &ptr, m_Clients.m_Seq)
	{
		if (!ptr) break;
		if (ptr->GetProcessType() == USER_LOOP)
			ptr->Proc();
	}

	// server user loop
	BOOST_FOREACH(auto &pSvr, m_Servers.m_Seq)
	{
		if (!pSvr) break;
		if (pSvr->GetProcessType() == USER_LOOP)
			pSvr->Proc();
	}
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CNetController::StartServer(int port, ServerBasicPtr pSvr)
{
	RETV(!pSvr, false);

	{ /// Sync
		common::AutoCSLock cs(m_CS); 	/// Sync
		Servers::iterator it = m_Servers.find(pSvr->GetNetId());
		if (m_Servers.end() != it)
		{
			clog::Error( clog::ERROR_WARNING, "�̹� ����ǰ� �ִ� ������ �ٽ� ���������\n");
			return false;
		}
	}

	if (pSvr->IsServerOn())
		pSvr->Close();

	if (!netlauncher::LaunchServer(pSvr, port))
		return false;

	// ���� ���ۿ� ���õ� �ڵ� �߰�
	clog::Log( clog::LOG_F_N_O, "%d Server Start\n", pSvr->GetNetId() );

	{ /// Sync
		common::AutoCSLock cs(m_CS); /// Sync
		m_Servers.insert( Servers::value_type(pSvr->GetNetId(), pSvr) );
		m_ServerSockets.insert( ServerSockets::value_type(pSvr->GetSocket(), pSvr) );
	}

	// Work ������ ����
	common::CThread *pWorkTread = AllocWorkThread(SERVER, pSvr.Get());
	if (pWorkTread)
	{
		pWorkTread->AddTask( new CTaskWorkServer((int)pSvr->GetSocket(), pSvr->GetNetId()) );
		pWorkTread->Start();
		pSvr->SetThreadHandle(pWorkTread->GetHandle()); // after Call Thread Start Function
	}

	return true;
}


//------------------------------------------------------------------------
// remove server container
// remove thread task
//------------------------------------------------------------------------
bool CNetController::StopServer(ServerBasicPtr pSvr)
{
	RETV(!pSvr, false);

	DisconnectServer(pSvr);
	return true;
}


/**
 @brief Remove Server
	Call from CServerBasic::Disconnect()
 */
bool	 CNetController::RemoveServer(ServerBasicPtr pSvr)
{
	RETV(!pSvr, false);

	common::AutoCSLock cs(m_CS); /// sync

	if (!m_Servers.remove(pSvr->GetNetId()))
		return false;

	ServerItor it = m_ServerSockets.find(pSvr->GetSocket());
	if (m_ServerSockets.end() != it)
		m_ServerSockets.erase(it);

	return true;
}


//------------------------------------------------------------------------
// netId �� �ش��ϴ� ������ �����Ѵ�.
//------------------------------------------------------------------------
ServerBasicPtr CNetController::GetServer(netid netId)
{
	common::AutoCSLock cs(m_CS); /// sync

	Servers::iterator it = m_Servers.find(netId);
	if (m_Servers.end() == it)
		return NULL;
	return it->second;
}


//------------------------------------------------------------------------
// SOCKET �� �ش��ϴ� ������ �����Ѵ�.
//------------------------------------------------------------------------
ServerBasicPtr CNetController::GetServerFromSocket(SOCKET sock)
{
	common::AutoCSLock cs(m_CS); /// sync

	ServerItor it = m_ServerSockets.find(sock);
	if (m_ServerSockets.end() == it)
		return NULL;
	return it->second;
}


//------------------------------------------------------------------------
// Ŭ���̾�Ʈ�� ip, port �� ������ ������ �õ��Ѵ�.
//------------------------------------------------------------------------
bool CNetController::StartClient(const std::string &ip, int port, ClientBasicPtr pClt)
{
	RETV(!pClt, false);

	if (pClt->IsConnect())
		pClt->Close(); // ������ ����

	clog::Log( clog::LOG_F_N_O, "%d Client Start\n", pClt->GetNetId() );
	if (!StartCoreClient(ip, port, pClt->GetConnectSvrClient()))
		return false;

	{ /// Sync
		common::AutoCSLock cs(m_CS); /// Sync
		Clients::iterator it = m_Clients.find( pClt->GetNetId());
		if (m_Clients.end() == it)
		{
			m_Clients.insert( Clients::value_type(pClt->GetNetId(), pClt) );
			m_ClientSockets.insert( ClientSockets::value_type(pClt->GetSocket(), pClt) );
		}
	}
	return true;
}


//------------------------------------------------------------------------
// Ŭ���̾�Ʈ�� �����Ѵ�.
//------------------------------------------------------------------------
bool CNetController::StopClient(ClientBasicPtr pClt)
{
	RETV(!pClt, false);

	DisconnectClient(pClt);
	return true;
}


/**
 @brief Remove Client
 Call from CClientBasic::Disconnect
 */
bool	CNetController::RemoveClient(ClientBasicPtr pClt)
{
	RETV(!pClt, false);

	common::AutoCSLock cs(m_CS); 	/// Sync

	if (!m_Clients.remove(pClt->GetNetId()))
		clog::Error( clog::ERROR_PROBLEM, "StopClient Error!! netid: %d client\n", pClt->GetNetId());

	ClientItor it = m_ClientSockets.find(pClt->GetSocket());
	if (m_ClientSockets.end() == it)
		return false;

	m_ClientSockets.erase(it);
	return true;
}


//------------------------------------------------------------------------
// clientId�� �ش��ϴ� Ŭ���̾�Ʈ�� �����Ѵ�.
//------------------------------------------------------------------------
ClientBasicPtr CNetController::GetClientFromSocket(SOCKET sock)
{
	common::AutoCSLock cs(m_CS); 	/// Sync

	ClientItor it = m_ClientSockets.find(sock);
	if (m_ClientSockets.end() == it)
		return NULL;
	return it->second;
}


//------------------------------------------------------------------------
// clientId�� �ش��ϴ� Ŭ���̾�Ʈ�� �����Ѵ�.
//------------------------------------------------------------------------
ClientBasicPtr CNetController::GetClient(netid netId)
{
	common::AutoCSLock cs(m_CS); 	/// Sync

	Clients::iterator it = m_Clients.find(netId);
	if (m_Clients.end() == it)
		return NULL;
	return it->second;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CNetController::StartCoreClient(const std::string &ip, int port, CoreClientPtr pClt)
{
	RETV(!pClt,false);

	if (pClt->IsConnect())
		pClt->Close(); // ������ ����

	// ���� ���ۿ� ���õ� �ڵ� �߰�
	clog::Log( clog::LOG_F_N_O, "%d Client Start\n", pClt->GetNetId() );

 	if (!netlauncher::LaunchCoreClient(pClt, ip, port))
 	{
 		clog::Error( clog::ERROR_CRITICAL, "StartCoreClient Error!! Launch Fail ip: %s, port: %d\n", ip.c_str(), port);
 		return false;
 	}

	{ /// Sync
		common::AutoCSLock cs(m_CS); 	/// Sync
		CoreClients::iterator it = m_CoreClients.find( pClt->GetNetId());
		if (m_CoreClients.end() == it)
			m_CoreClients.insert( CoreClients::value_type(pClt->GetNetId(), pClt) );
	}

	// CoreClient �Ӽ��� ���� Thread���� ��Ŷ�� ó������, ���� �������� ó������ �����Ѵ�.
	common::CThread *pWorkTread = AllocWorkThread(CLIENT, pClt.Get());
	if (pWorkTread)
	{
		if (SERVICE_EXCLUSIVE_THREAD == pClt->GetProcessType())
			pWorkTread->AddTask( new CTaskWork((int)pClt->GetSocket(), pClt->GetNetId(), pClt->GetSocket()) );
		pWorkTread->Start();
		pClt->SetThreadHandle(pWorkTread->GetHandle());
	}
	return true;
}


//------------------------------------------------------------------------
// remove coreclient list 
// remove task if SERVICE_EXCLUSIVE_THREAD mode coreclient
//------------------------------------------------------------------------
bool CNetController::StopCoreClient(CoreClientPtr pClt)
{
	RETV(!pClt, false);

	DisconnectCoreClient(pClt);
	return true;
}


/**
 @brief Remove CoreClient
 Call from CCoreClient::Disconnect()
 */
bool	CNetController::RemoveCoreClient(CoreClientPtr  pClt)
{
	RETV(!pClt, false);

	{ /// Sync
		common::AutoCSLock cs(m_CS); 	/// Sync
		if (!m_CoreClients.remove(pClt->GetNetId()))
			clog::Error( clog::ERROR_PROBLEM, "StopClient Error!! netid: %d client\n", pClt->GetNetId());
	}

	// Stop CoreClient Work Thread
	if (pClt->GetProcessType() == SERVICE_EXCLUSIVE_THREAD)
	{
		ThreadPtr ptr = GetThread( m_WorkThreads, pClt->GetThreadHandle() );
		if (ptr)
			ptr->Send2ThreadMessage( common::threadmsg::TERMINATE_TASK, pClt->GetSocket(), 0 );
	}
	return true;
}


//------------------------------------------------------------------------
// clientId�� �ش��ϴ� Ŭ���̾�Ʈ�� �����Ѵ�.
//------------------------------------------------------------------------
CoreClientPtr CNetController::GetCoreClient(netid netId)
{
	common::AutoCSLock cs(m_CS); 	/// Sync

	CoreClients::iterator it = m_CoreClients.find(netId);
	if (m_CoreClients.end() == it)
		return NULL;
	return it->second;
}


//------------------------------------------------------------------------
// Protocol Dispatcher �߰�
//------------------------------------------------------------------------
void CNetController::AddDispatcher(IProtocolDispatcher *pDispatcher)
{
	common::AutoCSLock cs(m_CS); 	/// Sync

	DispatcherItor it = m_Dispatchers.find(pDispatcher->GetId());
	if (m_Dispatchers.end() != it)
	{
		clog::Error( clog::ERROR_WARNING, 
			common::format( "���� ProtocolDispatcher�� �̹� ����߽��ϴ�. DispatcherId: %d\n", pDispatcher->GetId()) );
		return; // �̹� �����Ѵٸ� ����
	}
	m_Dispatchers.insert( DispatcherMap::value_type(pDispatcher->GetId(), pDispatcher) );
}


//------------------------------------------------------------------------
// Protocol Dispatcher ����
//------------------------------------------------------------------------
IProtocolDispatcher* CNetController::GetDispatcher(int protocolID)
{
	common::AutoCSLock cs(m_CS); 	/// Sync

	DispatcherItor it = m_Dispatchers.find(protocolID);
	if (m_Dispatchers.end() == it)
		return NULL; // ���ٸ� ����
	return it->second;
}


//------------------------------------------------------------------------
// �������� fd_set �� �����ؼ� �����Ѵ�.
//------------------------------------------------------------------------
void CNetController::MakeServersFDSET( fd_set *pfdset )
{
	common::AutoCSLock cs(m_CS);

	FD_ZERO(pfdset);
	BOOST_FOREACH(ServerSockets::value_type &kv, m_ServerSockets)
	{
		FD_SET( kv.second->GetSocket(), pfdset );
	}
}


//------------------------------------------------------------------------
// CoreClient �߿��� procType �� �ش��ϴ� CoreClient�鸸 fd_set�� �����Ѵ�.
//------------------------------------------------------------------------
void	CNetController::MakeCoreClientsFDSET( PROCESS_TYPE procType, SFd_Set *pfdset)
{
	common::AutoCSLock cs(m_CS);

	FD_ZERO(pfdset);
	BOOST_FOREACH(CoreClientPtr &ptr, m_CoreClients.m_Seq)
	{
		if (!ptr) continue;
		if (ptr->GetProcessType() == procType)
		{
			FD_SET( ptr->GetSocket(), pfdset );
			pfdset->netid_array[ pfdset->fd_count-1] = ptr->GetNetId();
		}
	}
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CNetController::Clear()
{
	m_AcceptThread.Terminate();

	BOOST_FOREACH( common::CThread *pThread, m_LogicThreads)
	{
		pThread->Terminate();
		delete pThread;
	}
	m_LogicThreads.clear();

	BOOST_FOREACH( common::CThread *pThread, m_WorkThreads)
	{
		pThread->Terminate();
		delete pThread;
	}
	m_WorkThreads.clear();

	m_pSeperateServerWorkThread = NULL;
	m_pSeperateClientWorkThread = NULL;

	CPacketQueue::Release();

}


//------------------------------------------------------------------------
// ��Ʈ������ ��ȯ, �ַ� ����뿡 ���õ� ������ ��Ʈ������ ��������.
//------------------------------------------------------------------------
std::string CNetController::ToString()
{
	std::stringstream ss;

	// ������ ����
	ss << "Thread.. " << std::endl;
	ss << m_AcceptThread.GetName() << " state: " << m_AcceptThread.GetState() << std::endl;

	BOOST_FOREACH( common::CThread *pThread, m_LogicThreads)
	{
		ss << pThread->GetName() << " state: " << pThread->GetState() << std::endl;
	}
	BOOST_FOREACH( common::CThread *pThread, m_WorkThreads)
	{
		ss << pThread->GetName() << " state: " << pThread->GetState() << std::endl;
	}	

	// ���� ����
	ss << std::endl;
	ss << "Server Cnt: " << m_ServerSockets.size() << std::endl;

	// �����İ���
	ss << "Dispatcher Cnt: " << m_Dispatchers.size() << std::endl;
	BOOST_FOREACH( DispatcherMap::value_type &kv, m_Dispatchers)
	{
		ss << "id: " << kv.second->GetId() << std::endl;
	}

	return ss.str();
}


//------------------------------------------------------------------------
// �ش�Ǵ� Ÿ���� WorkThread�� �����Ѵ�.
//------------------------------------------------------------------------
ThreadPtr CNetController::AllocWorkThread(SERVICE_TYPE serviceType, NetConnectorPtr pConnector)
{
	RETV(!pConnector, NULL);

	const PROCESS_TYPE processType = pConnector->GetProcessType();

	switch (serviceType)
	{
	case CLIENT:
		if (SERVICE_SEPERATE_THREAD == processType)
		{
			if (m_pSeperateClientWorkThread)
				return m_pSeperateClientWorkThread;

			common::CThread *pThread = new common::CThread("ClientWorkThread");
			m_pSeperateClientWorkThread = pThread;

			pThread->AddTask( new CTaskWorkClient(0) );
			m_WorkThreads.push_back(pThread);
			return pThread;
		}
		break;

	case SERVER:
		if (SERVICE_SEPERATE_THREAD == processType)
		{
			if (m_pSeperateServerWorkThread)
				return m_pSeperateServerWorkThread;

			common::CThread *pThread = new common::CThread("ServerWorkThread");
			m_pSeperateServerWorkThread = pThread;
			m_WorkThreads.push_back(pThread);
			return pThread;
		}
		break;
	}

	/// etc
	switch (processType)
	{
	case USER_LOOP: return NULL;
	case SERVICE_EXCLUSIVE_THREAD:
		{
			std::string threadName = (serviceType==SERVER)? "ServerWorkThread" : "ClientWorkThread";
			common::CThread *pThread = new common::CThread(threadName);
			m_WorkThreads.push_back(pThread);
			return pThread;
		}
		break;

	case SERVICE_CHILD_THREAD:
		{
			NetConnectorPtr pParent = pConnector->GetParent();
			if (!pParent)
				return NULL;
			ThreadPtr ptr = GetThread( m_WorkThreads, pParent->GetThreadHandle() );
			return ptr;
		}
		break;
	}

	return NULL;
}


//------------------------------------------------------------------------
// find WorkThread of hThreadHandle 
//------------------------------------------------------------------------
ThreadPtr CNetController::GetThread( const ThreadList &threads, HANDLE hThreadHandle )
{
	common::AutoCSLock cs(m_CS); 	/// Sync

	auto it = std::find_if( threads.begin(), threads.end(), 
		boost::bind( &common::IsSameHandle<common::CThread>, _1, hThreadHandle) );
	if (threads.end() == it)
		return NULL;
	return *it;
}


/**
 @brief Call Disconnect function through the thread message communication
 */
void	CNetController::DisconnectServer(ServerBasicPtr pSvr)
{
	RET(!pSvr);

	CPacketQueue::Get()->PushPacket( 
		CPacketQueue::SPacketData(pSvr->GetNetId(), 
			DisconnectPacket(pSvr->GetNetId(), GetUniqueValue()) ));

	switch (pSvr->GetProcessType())
	{
	case USER_LOOP: 
	case SERVICE_SEPERATE_THREAD:  
		break;

	case SERVICE_EXCLUSIVE_THREAD:
		{
			ThreadPtr ptr = GetThread( m_WorkThreads, pSvr->GetThreadHandle() );
			if (!ptr) break;
			ptr->Send2ThreadMessage( common::threadmsg::TERMINATE_TASK, pSvr->GetSocket(), 0 );
		}
		break;
	}
}


/**
 @brief Call Disconnect function through the thread message communication
 */
void	CNetController::DisconnectClient(ClientBasicPtr pClt)
{
	RET(!pClt);

	CPacketQueue::Get()->PushPacket( 
		CPacketQueue::SPacketData(pClt->GetNetId(), 
			DisconnectPacket(pClt->GetNetId(), GetUniqueValue()) ));
}


/**
 @brief Call Disconnect function through the thread message communication
 */
void	CNetController::DisconnectCoreClient(CoreClientPtr pCoreClt)
{
	RET(!pCoreClt);

	CPacketQueue::Get()->PushPacket( 
		CPacketQueue::SPacketData(pCoreClt->GetNetId(), 
			DisconnectPacket(pCoreClt->GetNetId(), GetUniqueValue()) ));

	switch (pCoreClt->GetProcessType())
	{
	case USER_LOOP: 
	case SERVICE_SEPERATE_THREAD: 
		break;

	// ���� �� case �� ȣ��� ���� ����. core client �� ���� user loop������ �����Ѵ�.
	case SERVICE_EXCLUSIVE_THREAD:
		{
			ThreadPtr ptr = GetThread( m_WorkThreads, pCoreClt->GetThreadHandle() );
			if (!ptr) break;
			ptr->Send2ThreadMessage( common::threadmsg::TERMINATE_TASK, pCoreClt->GetSocket(), 0 );
		}
		break;
	}
}
