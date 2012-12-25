
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
// logicThreadCount ������ŭ ���������带 �����Ѵ�.
//------------------------------------------------------------------------
bool CNetController::Init(int logicThreadCount)
{
	// ���������� ����
	for (int i=0; i < logicThreadCount; ++i)
	{
		common::CThread *pThread = new common::CThread();
		pThread->AddTask( new CTaskLogic() );
		pThread->Start();
		m_LogicThreads.push_back( pThread );
	}

	// Accept ������ ����
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
		error::ErrorLog("�̹� ����ǰ� �ִ� ������ �ٽ� ���������");
		return false;
	}

	// ���� ���ۿ� ���õ� �ڵ� �߰�
	error::Log( common::format("%d Server Start", pSvr->GetId()) );
	m_Servers.insert( ServerMap::value_type(pSvr->GetSocket(), pSvr) );

	// Work������ ����
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

	// ���� ���� �ڵ� �߰�
	return pSvr->Stop();
}


//------------------------------------------------------------------------
// serverid �� �ش��ϴ� ������ �����Ѵ�.
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
		return false; // �̹� �����Ѵٸ� ����

	// Ŭ���̾�Ʈ ���� �ڵ� �߰�
	// ���� ���ۿ� ���õ� �ڵ� �߰�
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

	// Ŭ���̾�Ʈ ���� �ڵ� �߰�
	return pClt->Stop();
}


//------------------------------------------------------------------------
// clientId�� �ش��ϴ� Ŭ���̾�Ʈ�� �����Ѵ�.
//------------------------------------------------------------------------
CClient* CNetController::GetClient(SOCKET sock)
{
	ClientItor it = m_Clients.find(sock);
	if (m_Clients.end() == it)
		return NULL;
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
		BOOST_FOREACH(ServerMap::value_type &kv, m_Servers)
		{
			pfdset->fd_array[ pfdset->fd_count] = kv.second->GetSocket();
			++pfdset->fd_count;
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

