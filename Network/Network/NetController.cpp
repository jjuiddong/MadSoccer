
#include "stdafx.h"
#include "NetController.h"
#include "NetLauncher.h"
#include "Server.h"
#include "Client.h"
#include "Task/TaskLogic.h"
#include "task/TaskAccept.h"
#include "task/TaskWork.h"


using namespace network;


CNetController::CNetController() 
{
	InitializeCriticalSection(&m_CriticalSection);
}

CNetController::~CNetController() 
{
	CPacketQueue::Release();

	DeleteCriticalSection(&m_CriticalSection);

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

}


//------------------------------------------------------------------------
// logicThreadCount 갯수만큼 로직쓰레드를 생성한다.
//------------------------------------------------------------------------
bool CNetController::Init(int logicThreadCount)
{
	// 로직쓰레드 생성
	for (int i=0; i < logicThreadCount; ++i)
	{
		common::CThread *pThread = new common::CThread();
		pThread->AddTask( new CTaskLogic() );
		pThread->Start();
		m_LogicThreads.push_back( pThread );
	}

	// Accept 쓰레드 생성
	m_AcceptThread.AddTask( new CTaskAccept() );
	m_AcceptThread.Start();

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
bool CNetController::StartServer(int port, CServer *pSvr)
{
	if (!pSvr)
		return false;

	if (!CNetLauncher::Get()->LaunchServer(pSvr, port))
		return false;

	ServerItor it = m_Servers.find(pSvr->GetSocket());
	if (m_Servers.end() != it)
	{
		error::ErrorLog("이미 실행되고 있는 서버를 다시 실행시켰음");
		return false;
	}

	// 서버 시작에 관련된 코드 추가
	error::Log( common::format("%d Server Start", pSvr->GetId()) );
	m_Servers.insert( ServerMap::value_type(pSvr->GetSocket(), pSvr) );

	// Work쓰레드 생성
	common::CThread *pWorkTread = new common::CThread();
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
CServer* CNetController::GetServer(SOCKET sock)
{
	ServerItor it = m_Servers.find(sock);
	if (m_Servers.end() == it)
		return NULL;
	return it->second;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CNetController::StartClient(const std::string &ip, int port, CClient *pClt)
{
	if (!pClt)
		return false;

	if (!CNetLauncher::Get()->LaunchClient(pClt, ip, port))
		return false;

	ClientItor it = m_Clients.find(pClt->GetSocket());
	if (m_Clients.end() != it)
		return false; // 이미 존재한다면 실패

	// 클라이언트 접속 코드 추가
	// 서버 시작에 관련된 코드 추가
	error::Log( common::format("%d Client Start", pClt->GetId()) );
	m_Clients.insert( ClientMap::value_type(pClt->GetSocket(), pClt) );

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
CClient* CNetController::GetClient(SOCKET sock)
{
	ClientItor it = m_Clients.find(sock);
	if (m_Clients.end() == it)
		return NULL;
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

