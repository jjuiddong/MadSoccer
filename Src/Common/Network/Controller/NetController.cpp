
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
// logicThreadCount 갯수만큼 로직쓰레드를 생성한다.
//------------------------------------------------------------------------
bool CNetController::Init(int logicThreadCount)
{
	// 로직쓰레드 생성
	for (int i=0; i < logicThreadCount; ++i)
	{
		common::CThread *pThread = new common::CThread("LogicThread");
		pThread->AddTask( new CTaskLogic() );
		pThread->Start();
		m_LogicThreads.push_back( pThread );
	}

	// Accept 쓰레드 생성
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
			clog::Error( clog::ERROR_WARNING, "이미 실행되고 있는 서버를 다시 실행시켰음\n");
			return false;
		}
	}

	if (pSvr->IsServerOn())
		pSvr->Close();

	if (!netlauncher::LaunchServer(pSvr, port))
		return false;

	// 서버 시작에 관련된 코드 추가
	clog::Log( clog::LOG_F_N_O, "%d Server Start\n", pSvr->GetNetId() );

	{ /// Sync
		common::AutoCSLock cs(m_CS); /// Sync
		m_Servers.insert( Servers::value_type(pSvr->GetNetId(), pSvr) );
		m_ServerSockets.insert( ServerSockets::value_type(pSvr->GetSocket(), pSvr) );
	}

	// Work 쓰레드 생성
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
// netId 에 해당하는 서버를 리턴한다.
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
// SOCKET 에 해당하는 서버를 리턴한다.
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
// 클라이언트는 ip, port 의 서버에 접속을 시도한다.
//------------------------------------------------------------------------
bool CNetController::StartClient(const std::string &ip, int port, ClientBasicPtr pClt)
{
	RETV(!pClt, false);

	if (pClt->IsConnect())
		pClt->Close(); // 연결을 끊고

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
// 클라이언트를 종료한다.
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
// clientId에 해당하는 클라이언트를 리턴한다.
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
// clientId에 해당하는 클라이언트를 리턴한다.
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
		pClt->Close(); // 연결을 끊고

	// 서버 시작에 관련된 코드 추가
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

	// CoreClient 속성에 따라 Thread에서 패킷을 처리할지, 유저 루프에서 처리할지 결정한다.
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
// clientId에 해당하는 클라이언트를 리턴한다.
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
// Protocol Dispatcher 추가
//------------------------------------------------------------------------
void CNetController::AddDispatcher(IProtocolDispatcher *pDispatcher)
{
	common::AutoCSLock cs(m_CS); 	/// Sync

	DispatcherItor it = m_Dispatchers.find(pDispatcher->GetId());
	if (m_Dispatchers.end() != it)
	{
		clog::Error( clog::ERROR_WARNING, 
			common::format( "같은 ProtocolDispatcher를 이미 등록했습니다. DispatcherId: %d\n", pDispatcher->GetId()) );
		return; // 이미 존재한다면 실패
	}
	m_Dispatchers.insert( DispatcherMap::value_type(pDispatcher->GetId(), pDispatcher) );
}


//------------------------------------------------------------------------
// Protocol Dispatcher 얻음
//------------------------------------------------------------------------
IProtocolDispatcher* CNetController::GetDispatcher(int protocolID)
{
	common::AutoCSLock cs(m_CS); 	/// Sync

	DispatcherItor it = m_Dispatchers.find(protocolID);
	if (m_Dispatchers.end() == it)
		return NULL; // 없다면 실패
	return it->second;
}


//------------------------------------------------------------------------
// 서버들의 fd_set 을 생성해서 리턴한다.
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
// CoreClient 중에서 procType 에 해당하는 CoreClient들만 fd_set을 구성한다.
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
// 스트링으로 변환, 주로 디버깅에 관련된 정보를 스트링으로 내보낸다.
//------------------------------------------------------------------------
std::string CNetController::ToString()
{
	std::stringstream ss;

	// 쓰레드 상태
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

	// 서버 갯수
	ss << std::endl;
	ss << "Server Cnt: " << m_ServerSockets.size() << std::endl;

	// 디스패쳐갯수
	ss << "Dispatcher Cnt: " << m_Dispatchers.size() << std::endl;
	BOOST_FOREACH( DispatcherMap::value_type &kv, m_Dispatchers)
	{
		ss << "id: " << kv.second->GetId() << std::endl;
	}

	return ss.str();
}


//------------------------------------------------------------------------
// 해당되는 타입의 WorkThread를 리턴한다.
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

	// 아직 이 case 가 호출될 일은 없다. core client 는 현재 user loop에서만 동작한다.
	case SERVICE_EXCLUSIVE_THREAD:
		{
			ThreadPtr ptr = GetThread( m_WorkThreads, pCoreClt->GetThreadHandle() );
			if (!ptr) break;
			ptr->Send2ThreadMessage( common::threadmsg::TERMINATE_TASK, pCoreClt->GetSocket(), 0 );
		}
		break;
	}
}
