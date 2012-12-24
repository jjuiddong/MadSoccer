
#include "stdafx.h"
#include "LogicThreadAllocator.h"
#include "Task/TaskLogic.h"
#include <boost/foreach.hpp>


using namespace network;


CLogicThreadAllocator::CLogicThreadAllocator() :
	m_LogicThreadCount(0)
{
	InitializeCriticalSection(&m_CriticalSection);
}

CLogicThreadAllocator::~CLogicThreadAllocator()
{
	Clear();
}


//------------------------------------------------------------------------
// 클래스 초기화
// logicThreadCount갯수만큼 로직쓰레드가 생성된다.
//------------------------------------------------------------------------
void CLogicThreadAllocator::Init( int logicThreadCount )
{
	m_LogicThreadCount = logicThreadCount;
	for (int i=0; i < m_LogicThreadCount; ++i)
	{
		common::CThread *pThread = new common::CThread();
		pThread->AddTask( new CTaskLogic() );
		pThread->Start();
		m_Threads.push_back( pThread );
	}
}


//------------------------------------------------------------------------
// 패킷을 저장한다.
//------------------------------------------------------------------------
void CLogicThreadAllocator::PushPacket(const SPacketData &data)
{
	EnterSync();
	{
		m_Packets.push_back(data);
	}
	LeaveSync();
}


//------------------------------------------------------------------------
// 저장된 패킷이 있다면 복사해서 넘겨준다. 
// 넘겨준 패킷은 제거된다.
//------------------------------------------------------------------------
bool CLogicThreadAllocator::PopPacket(OUT SPacketData &data)
{
	EnterSync();
	{
		if (m_Packets.empty())
		{
			LeaveSync();
			return false;
		}

		data = m_Packets.front();
		m_Packets.pop_front();
	}
	LeaveSync();
	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CLogicThreadAllocator::EnterSync()
{
	EnterCriticalSection(&m_CriticalSection);
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CLogicThreadAllocator::LeaveSync()
{
	LeaveCriticalSection(&m_CriticalSection);
}


//------------------------------------------------------------------------
// 등록된 모든 쓰레드를 제거한다.
//------------------------------------------------------------------------
void CLogicThreadAllocator::Clear()
{
	DeleteCriticalSection(&m_CriticalSection);

	BOOST_FOREACH( common::CThread *pThread, m_Threads)
	{
		pThread->Terminate();
		delete pThread;
	}
}
