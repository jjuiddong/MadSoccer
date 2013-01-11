
#include "stdafx.h"
#include "NetController.h"
#include "NetLauncher.h"
#include "../Service/Server.h"
#include "../Service/Client.h"
#include "../Task/TaskLogic.h"
#include "../task/TaskAccept.h"
#include "../task/TaskWork.h"


using namespace network;


CNetController::CNetController() :
	m_AcceptThread("AcceptThread")
{
	InitializeCriticalSection(&m_CriticalSection);
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

	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CNetController::Proc()
{
	ClientItor cit = m_Clients.begin();
	while (m_Clients.end() != cit)
	{
		CClient *pclt = cit++->second;
		pclt->Proc();
	}
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CNetController::StartServer(int port, ServerPtr pSvr)
{
	if (!pSvr)
		return false;

	ServerItor it = m_Servers.find(pSvr->GetSocket());
	if (m_Servers.end() != it)
	{
		error::ErrorLog("이미 실행되고 있는 서버를 다시 실행시켰음");
		return false;
	}

	if (!CNetLauncher::Get()->LaunchServer(pSvr, port))
		return false;

	// 서버 시작에 관련된 코드 추가
	clog::Log( "%d Server Start", pSvr->GetNetId() );
	dbg::Print( "%d Server Start", pSvr->GetNetId() );
	m_Servers.insert( ServerMap::value_type(pSvr->GetSocket(), pSvr) );

	// Work쓰레드 생성
	common::CThread *pWorkTread = new common::CThread("WorkThread");
	pWorkTread->AddTask( new CTaskWork(pSvr) );
	pWorkTread->Start();
	m_WorkThreads.push_back(pWorkTread);

	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CNetController::StopServer(CServer *pSvr)
{
	if (!pSvr)
		return false;

	ServerItor it = m_Servers.find(pSvr->GetSocket());
	if (m_Servers.end() == it)
		return false;

	m_Servers.erase(it);

	// 서버 종료 코드 추가
	return pSvr->Stop();
}


//------------------------------------------------------------------------
// serverid 에 해당하는 서버를 리턴한다.
//------------------------------------------------------------------------
ServerPtr CNetController::GetServer(SOCKET sock)
{
	ServerItor it = m_Servers.find(sock);
	if (m_Servers.end() == it)
		return NULL;
	return it->second;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CNetController::StartClient(const std::string &ip, int port, ClientPtr pClt)
{
	if (!pClt)
		return false;

	if (pClt->IsConnect())
		pClt->Stop(); // 연결을 끊고

	// 서버 시작에 관련된 코드 추가
	clog::Log( "%d Client Start", pClt->GetNetId() );
	dbg::Print( "%d Client Start", pClt->GetNetId() );
	if (!CNetLauncher::Get()->LaunchClient(pClt, ip, port))
		return false;

	ClientItor it = m_Clients.find(pClt->GetSocket());
	if (m_Clients.end() == it)
	{
		m_Clients.insert( ClientMap::value_type(pClt->GetSocket(), pClt) );
	}

	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CNetController::StopClient(CClient *pClt)
{
	if (!pClt)
		return false;

	ClientItor it = m_Clients.find(pClt->GetSocket());
	if (m_Clients.end() == it)
		return false;

	m_Clients.erase(it);

	// 클라이언트 종료 코드 추가
	return pClt->Stop();
}


//------------------------------------------------------------------------
// clientId에 해당하는 클라이언트를 리턴한다.
//------------------------------------------------------------------------
ClientPtr CNetController::GetClient(SOCKET sock)
{
	ClientItor it = m_Clients.find(sock);
	if (m_Clients.end() == it)
		return NULL;
	return it->second;
}


//------------------------------------------------------------------------
// Protocol Dispatcher 추가
//------------------------------------------------------------------------
void CNetController::AddDispatcher(IProtocolDispatcher *pDispatcher)
{
	DispatcherItor it = m_Dipatchers.find(pDispatcher->GetId());
	if (m_Dipatchers.end() != it)
	{
		error::ErrorLog( 
			common::format( "같은 ProtocolDispatcher를 이미 등록했습니다. DispatcherId: %d ", pDispatcher->GetId()) );
		return; // 이미 존재한다면 실패
	}
	m_Dipatchers.insert( DispatcherMap::value_type(pDispatcher->GetId(), pDispatcher) );
}


//------------------------------------------------------------------------
// Protocol Dispatcher 얻음
//------------------------------------------------------------------------
IProtocolDispatcher* CNetController::GetDispatcher(int protocolID)
{
	DispatcherItor it = m_Dipatchers.find(protocolID);
	if (m_Dipatchers.end() == it)
		return NULL; // 없다면 실패
	return it->second;
}


//------------------------------------------------------------------------
// 서버들의 fd_set 을 생성해서 리턴한다.
//------------------------------------------------------------------------
void CNetController::MakeServersFDSET( fd_set *pfdset )
{
	EnterSync();
	{
		FD_ZERO(pfdset);
		BOOST_FOREACH(ServerMap::value_type &kv, m_Servers)
		{
			pfdset->fd_array[ pfdset->fd_count] = kv.second->GetSocket();
			++pfdset->fd_count;
		}
	}
	LeaveSync();
}


//------------------------------------------------------------------------
// 동기화 시작
//------------------------------------------------------------------------
void CNetController::EnterSync()
{
	EnterCriticalSection( &m_CriticalSection );
}


//------------------------------------------------------------------------
// 동기화 끝
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

	CPacketQueue::Release();

	DeleteCriticalSection(&m_CriticalSection);

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
	ss << "Server Cnt: " << m_Servers.size() << std::endl;

	// 디스패쳐갯수
	ss << "Dispatcher Cnt: " << m_Dipatchers.size() << std::endl;
	BOOST_FOREACH( DispatcherMap::value_type &kv, m_Dipatchers)
	{
		ss << "id: " << kv.second->GetId() << std::endl;
	}

	return ss.str();
}
