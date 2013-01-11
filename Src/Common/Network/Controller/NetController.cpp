
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
		error::ErrorLog("�̹� ����ǰ� �ִ� ������ �ٽ� ���������");
		return false;
	}

	if (!CNetLauncher::Get()->LaunchServer(pSvr, port))
		return false;

	// ���� ���ۿ� ���õ� �ڵ� �߰�
	clog::Log( "%d Server Start", pSvr->GetNetId() );
	dbg::Print( "%d Server Start", pSvr->GetNetId() );
	m_Servers.insert( ServerMap::value_type(pSvr->GetSocket(), pSvr) );

	// Work������ ����
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

	// ���� ���� �ڵ� �߰�
	return pSvr->Stop();
}


//------------------------------------------------------------------------
// serverid �� �ش��ϴ� ������ �����Ѵ�.
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
		pClt->Stop(); // ������ ����

	// ���� ���ۿ� ���õ� �ڵ� �߰�
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

	// Ŭ���̾�Ʈ ���� �ڵ� �߰�
	return pClt->Stop();
}


//------------------------------------------------------------------------
// clientId�� �ش��ϴ� Ŭ���̾�Ʈ�� �����Ѵ�.
//------------------------------------------------------------------------
ClientPtr CNetController::GetClient(SOCKET sock)
{
	ClientItor it = m_Clients.find(sock);
	if (m_Clients.end() == it)
		return NULL;
	return it->second;
}


//------------------------------------------------------------------------
// Protocol Dispatcher �߰�
//------------------------------------------------------------------------
void CNetController::AddDispatcher(IProtocolDispatcher *pDispatcher)
{
	DispatcherItor it = m_Dipatchers.find(pDispatcher->GetId());
	if (m_Dipatchers.end() != it)
	{
		error::ErrorLog( 
			common::format( "���� ProtocolDispatcher�� �̹� ����߽��ϴ�. DispatcherId: %d ", pDispatcher->GetId()) );
		return; // �̹� �����Ѵٸ� ����
	}
	m_Dipatchers.insert( DispatcherMap::value_type(pDispatcher->GetId(), pDispatcher) );
}


//------------------------------------------------------------------------
// Protocol Dispatcher ����
//------------------------------------------------------------------------
IProtocolDispatcher* CNetController::GetDispatcher(int protocolID)
{
	DispatcherItor it = m_Dipatchers.find(protocolID);
	if (m_Dipatchers.end() == it)
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
	ss << "Server Cnt: " << m_Servers.size() << std::endl;

	// �����İ���
	ss << "Dispatcher Cnt: " << m_Dipatchers.size() << std::endl;
	BOOST_FOREACH( DispatcherMap::value_type &kv, m_Dipatchers)
	{
		ss << "id: " << kv.second->GetId() << std::endl;
	}

	return ss.str();
}
