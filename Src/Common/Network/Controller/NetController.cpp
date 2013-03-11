
#include "stdafx.h"
#include "NetController.h"
#include "NetLauncher.h"
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
	InitializeCriticalSection(&m_CriticalSection);
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

		// CoreClient�� ���� WorkThread ����
		//ThreadPtr pWorkThread = CreateWorkThread( CLIENT, SERVICE_SEPERATE_THREAD );
		//if (pWorkThread)
		//	pWorkThread->Start();
	}

	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CNetController::Proc()
{
	BOOST_FOREACH(ClientPtr &ptr, m_Clients.m_Seq)
	{
		if (!ptr) break;
		ptr->Proc();
	}
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CNetController::StartServer(int port, ServerPtr pSvr)
{
	if (!pSvr)
		return false;

	Servers::iterator it = m_Servers.find(pSvr->GetNetId());
	if (m_Servers.end() != it)
	{
		error::ErrorLog("�̹� ����ǰ� �ִ� ������ �ٽ� ���������");
		return false;
	}

	if (pSvr->IsServerOn())
		pSvr->Disconnect();

	if (!CNetLauncher::Get()->LaunchServer(pSvr, port))
		return false;

	// ���� ���ۿ� ���õ� �ڵ� �߰�
	LogNPrint( "%d Server Start", pSvr->GetNetId() );
	m_Servers.insert( Servers::value_type(pSvr->GetNetId(), pSvr) );
	m_ServerSockets.insert( ServerSockets::value_type(pSvr->GetSocket(), pSvr) );

	// Work ������ ����
	common::CThread *pWorkTread = CreateWorkThread(SERVER, pSvr->GetProcessType());
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
bool CNetController::StopServer(ServerPtr pSvr)
{
	if (!pSvr)
		return false;

	if (!m_Servers.remove(pSvr->GetNetId()))
		return false;

	ServerItor it = m_ServerSockets.find(pSvr->GetSocket());
	if (m_ServerSockets.end() != it)
		m_ServerSockets.erase(it);

	// Stop Server Work Thread
	if (pSvr->GetProcessType() == SERVICE_EXCLUSIVE_THREAD)
	{
		ThreadPtr ptr = GetThread( m_WorkThreads, pSvr->GetThreadHandle() );
		if (ptr)
			ptr->Send2ThreadMessage( common::threadmsg::TERMINATE_TASK, pSvr->GetSocket(), 0 );
	}
	return true;
}


//------------------------------------------------------------------------
// netId �� �ش��ϴ� ������ �����Ѵ�.
//------------------------------------------------------------------------
ServerPtr CNetController::GetServer(netid netId)
{
	Servers::iterator it = m_Servers.find(netId);
	if (m_Servers.end() == it)
		return NULL;
	return it->second;
}


//------------------------------------------------------------------------
// SOCKET �� �ش��ϴ� ������ �����Ѵ�.
//------------------------------------------------------------------------
ServerPtr CNetController::GetServerFromSocket(SOCKET sock)
{
	ServerItor it = m_ServerSockets.find(sock);
	if (m_ServerSockets.end() == it)
		return NULL;
	return it->second;
}


//------------------------------------------------------------------------
// Ŭ���̾�Ʈ�� ip, port �� ������ ������ �õ��Ѵ�.
//------------------------------------------------------------------------
bool CNetController::StartClient(const std::string &ip, int port, ClientPtr pClt)
{
	if (!pClt)
		return false;

	if (pClt->IsConnect())
		pClt->Disconnect(); // ������ ����

	LogNPrint( "%d Client Start", pClt->GetNetId() );
	if (!StartCoreClient(ip, port, pClt->GetConnectSvrClient()))
		return false;

	Clients::iterator it = m_Clients.find( pClt->GetNetId());
	if (m_Clients.end() != it)
	{
		m_Clients.insert( Clients::value_type(pClt->GetNetId(), pClt) );
		m_ClientSockets.insert( ClientSockets::value_type(pClt->GetSocket(), pClt) );
	}
	return true;
}


//------------------------------------------------------------------------
// Ŭ���̾�Ʈ�� �����Ѵ�.
//------------------------------------------------------------------------
bool CNetController::StopClient(ClientPtr pClt)
{
	if (!pClt)
		return false;

	if (!m_Clients.remove(pClt->GetNetId()))
		LogNPrint( "StopClient Error!! netid: %d client", pClt->GetNetId());

	ClientItor it = m_ClientSockets.find(pClt->GetSocket());
	if (m_ClientSockets.end() == it)
		return false;

	m_ClientSockets.erase(it);
	return true;
}


//------------------------------------------------------------------------
// clientId�� �ش��ϴ� Ŭ���̾�Ʈ�� �����Ѵ�.
//------------------------------------------------------------------------
ClientPtr CNetController::GetClientFromSocket(SOCKET sock)
{
	ClientItor it = m_ClientSockets.find(sock);
	if (m_ClientSockets.end() == it)
		return NULL;
	return it->second;
}


//------------------------------------------------------------------------
// clientId�� �ش��ϴ� Ŭ���̾�Ʈ�� �����Ѵ�.
//------------------------------------------------------------------------
ClientPtr CNetController::GetClient(netid netId)
{
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
	if (!pClt)
		return false;

	if (pClt->IsConnect())
		pClt->Disconnect(); // ������ ����

	// ���� ���ۿ� ���õ� �ڵ� �߰�
	LogNPrint( "%d Client Start", pClt->GetNetId() );
 	if (!CNetLauncher::Get()->LaunchCoreClient(pClt, ip, port))
 	{
 		LogNPrint( "StartCoreClient Error!! Launch Fail ip: %s, port: %d", ip.c_str(), port);
 		return false;
 	}

	CoreClients::iterator it = m_CoreClients.find( pClt->GetNetId());
	if (m_CoreClients.end() != it)
	{
		m_CoreClients.insert( CoreClients::value_type(pClt->GetNetId(), pClt) );
	}

	// CoreClient �Ӽ��� ���� Thread���� ��Ŷ�� ó������, ���� �������� ó������ �����Ѵ�.
	common::CThread *pWorkTread = CreateWorkThread(CLIENT, pClt->GetProcessType());
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
	if (!pClt)
		return false;

	if (!m_CoreClients.remove(pClt->GetNetId()))
		LogNPrint( "StopClient Error!! netid: %d client", pClt->GetNetId());

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
	DispatcherItor it = m_Dispatchers.find(pDispatcher->GetId());
	if (m_Dispatchers.end() != it)
	{
		error::ErrorLog( 
			common::format( "���� ProtocolDispatcher�� �̹� ����߽��ϴ�. DispatcherId: %d ", pDispatcher->GetId()) );
		return; // �̹� �����Ѵٸ� ����
	}
	m_Dispatchers.insert( DispatcherMap::value_type(pDispatcher->GetId(), pDispatcher) );
}


//------------------------------------------------------------------------
// Protocol Dispatcher ����
//------------------------------------------------------------------------
IProtocolDispatcher* CNetController::GetDispatcher(int protocolID)
{
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
	EnterSync();
	{
		FD_ZERO(pfdset);
		BOOST_FOREACH(ServerSockets::value_type &kv, m_ServerSockets)
		{
			FD_SET( kv.second->GetSocket(), pfdset );
			//pfdset->fd_array[ pfdset->fd_count] = kv.second->GetSocket();
			//++pfdset->fd_count;
		}
	}
	LeaveSync();
}


//------------------------------------------------------------------------
// CoreClient �߿��� procType �� �ش��ϴ� CoreClient�鸸 fd_set�� �����Ѵ�.
//------------------------------------------------------------------------
void	CNetController::MakeCoreClientsFDSET( PROCESS_TYPE procType, SFd_Set *pfdset)
{
	EnterSync();
	{
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
	LeaveSync();
}


//------------------------------------------------------------------------
// ����ȭ ����
//------------------------------------------------------------------------
void CNetController::EnterSync()
{
	EnterCriticalSection( &m_CriticalSection );
}


//------------------------------------------------------------------------
// ����ȭ ��
//------------------------------------------------------------------------
void CNetController::LeaveSync()
{
	LeaveCriticalSection( &m_CriticalSection );
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

	DeleteCriticalSection(&m_CriticalSection);

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
ThreadPtr CNetController::CreateWorkThread(SERVICE_TYPE serviceType, PROCESS_TYPE processType)
{
	switch (serviceType)
	{
	case CLIENT:
		switch (processType)
		{
		case USER_LOOP: return NULL;
		case SERVICE_SEPERATE_THREAD:
			{
				if (m_pSeperateClientWorkThread)
					return m_pSeperateClientWorkThread;

				common::CThread *pThread = new common::CThread("ClientWorkThread");
				m_pSeperateClientWorkThread = pThread;
				m_WorkThreads.push_back(pThread);
				return pThread;
			}
			break;
		case SERVICE_EXCLUSIVE_THREAD:
			{
				common::CThread *pThread = new common::CThread("ClientWorkThread");
				m_WorkThreads.push_back(pThread);
				return pThread;
			}
			break;
		}
		break;

	case SERVER:
		switch (processType)
		{
		case USER_LOOP: return NULL;
		case SERVICE_SEPERATE_THREAD:
			{
				if (m_pSeperateServerWorkThread)
					return m_pSeperateServerWorkThread;

				common::CThread *pThread = new common::CThread("ServerWorkThread");
				m_pSeperateServerWorkThread = pThread;
				m_WorkThreads.push_back(pThread);
				return pThread;
			}
			break;
		case SERVICE_EXCLUSIVE_THREAD:
			{
				common::CThread *pThread = new common::CThread("ServerWorkThread");
				m_WorkThreads.push_back(pThread);
				return pThread;
			}
			break;
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
	auto it = std::find_if( threads.begin(), threads.end(), 
		boost::bind( &common::IsSameHandle<common::CThread>, _1, hThreadHandle) );
	if (threads.end() == it)
		return NULL;
	return *it;
}
