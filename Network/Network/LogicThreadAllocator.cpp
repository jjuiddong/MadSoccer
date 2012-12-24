
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
// Ŭ���� �ʱ�ȭ
// logicThreadCount������ŭ ���������尡 �����ȴ�.
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
// ��Ŷ�� �����Ѵ�.
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
// ����� ��Ŷ�� �ִٸ� �����ؼ� �Ѱ��ش�. 
// �Ѱ��� ��Ŷ�� ���ŵȴ�.
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
// ��ϵ� ��� �����带 �����Ѵ�.
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
