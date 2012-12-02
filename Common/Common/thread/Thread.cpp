#include "stdafx.h"
#include "Thread.h"
#include "Task.h"
#include <process.h>


using namespace common;

namespace common
{
	void ThreadProcess(void *pThreadPtr )
	{
		CThread *pThread = (CThread*)pThreadPtr;
		pThread->Run();
	}

}


CThread::CThread() :
	m_State(WAIT)
{

}

CThread::~CThread()
{
	Clear();

}


//------------------------------------------------------------------------
//  ������ ����
//------------------------------------------------------------------------
void CThread::Start()
{
	InitializeCriticalSection( &m_TaskCriticalSection );
	InitializeCriticalSection( &m_MsgCriticalSection );

	// ������ ����
	m_State = RUN;
	_beginthread(ThreadProcess, 0, this);
}


//------------------------------------------------------------------------
// ������ ����
//------------------------------------------------------------------------
void CThread::Kill()
{
	m_State = END;
}


//------------------------------------------------------------------------
// �����忡�� �޼����� ������.
// rcvTaskId : ���� �½�ũ ���̵� ('0' �̶�� �����尡 �޴´�.)
//			   -1 : �ܺη� ���� �޼����� ����
//------------------------------------------------------------------------
void CThread::Send2ThreadMessage( int rcvTaskId, int msg, WPARAM wParam, LPARAM lParam)
{
	EnterMsgSync();
	m_ThreadMsgs.push_back( SExternalMsg(rcvTaskId, msg, wParam, lParam) );
	LeaveMsgSync();
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CThread::Send2ExternalMessage( int msg, WPARAM wParam, LPARAM lParam )
{
	EnterMsgSync();
	m_ExternalMsgs.push_back( SExternalMsg(-1, msg, wParam, lParam) );
	LeaveMsgSync();
}


//------------------------------------------------------------------------
// Task ����ȭ
//------------------------------------------------------------------------
void CThread::EnterTaskSync()
{
	EnterCriticalSection( &m_TaskCriticalSection );
}
//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CThread::LeaveTaskSync()
{
	LeaveCriticalSection( &m_TaskCriticalSection );
}
//------------------------------------------------------------------------
// Msg ����ȭ
//------------------------------------------------------------------------
void CThread::EnterMsgSync()
{
	EnterCriticalSection( &m_MsgCriticalSection );
}
//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CThread::LeaveMsgSync()
{
	LeaveCriticalSection( &m_MsgCriticalSection );
}


//------------------------------------------------------------------------
// �����尡 ���� �޼����� �����Ѵ�.
// �޼����� ���ٸ� false�� �����Ѵ�.
//------------------------------------------------------------------------
bool CThread::GetThreadMsg( OUT SExternalMsg *pMsg, MSG_OPT opt ) // opt = MSG_REMOVE
{
	if (!pMsg) 
		return false;

	bool reval;
	EnterMsgSync();
	{
		if (m_ThreadMsgs.empty())
		{
			reval = false;
		}
		else
		{
			*pMsg = m_ThreadMsgs.front();
			if (MSG_REMOVE == opt)
				m_ThreadMsgs.pop_front();
			reval = true;
		}
	}
	LeaveMsgSync();
	return reval;
}


//------------------------------------------------------------------------
// �����忡�� �ܺη� ���ϴ� �޼����� �����Ѵ�.
// �޼����� ���ٸ� false�� �����Ѵ�.
//------------------------------------------------------------------------
bool CThread::GetExternalMsg( OUT SExternalMsg *pMsg, MSG_OPT opt ) // opt = MSG_REMOVE
{
	if (!pMsg) 
		return false;

	bool reval;
	EnterMsgSync();
	{
		if (m_ExternalMsgs.empty())
		{
			reval = false;
		}
		else
		{
			*pMsg = m_ExternalMsgs.front();
			if (MSG_REMOVE == opt)
				m_ExternalMsgs.pop_front();
			reval = true;
		}
	}
	LeaveMsgSync();
	return reval;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CThread::AddTask(CTask *pTask)
{
	if (!pTask)
		return false;
	EnterTaskSync();
	{
		TaskItor it = find_if(m_Tasks.begin(), m_Tasks.end(), IsTask(pTask->GetId()));
		if (m_Tasks.end() != it)
			return false; // �̹� �����Ѵٸ� ����

		pTask->SetThread(this);
		m_Tasks.push_back( pTask );
	}
	LeaveTaskSync();
	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CThread::RemoveTask(CTask *pTask)
{
	if (!pTask)
		return false;
	EnterTaskSync();
	{
		TaskItor it = find_if(m_Tasks.begin(), m_Tasks.end(), IsTask(pTask->GetId()));
		if (m_Tasks.end() == it)
			return false; // ���ٸ� ����

		m_Tasks.remove_if( IsTask(pTask->GetId()) );
	}
	LeaveTaskSync();
	return true;
}


//------------------------------------------------------------------------
// �������� ������ Ŭ���� ����
//------------------------------------------------------------------------
void CThread::Clear()
{
	EnterTaskSync();
	{
		TaskItor it = m_Tasks.begin();
		while (m_Tasks.end() != it)
		{
			CTask *p = *it++;
			delete p;
		}
		m_Tasks.clear();
	}
	LeaveTaskSync();

	EnterMsgSync();
	{
		m_ThreadMsgs.clear();
		m_ExternalMsgs.clear();
	}
	LeaveMsgSync();

	DeleteCriticalSection( &m_TaskCriticalSection );
	DeleteCriticalSection( &m_MsgCriticalSection );

}


//------------------------------------------------------------------------
// ������ ����
// Task�� �����Ų��.
//------------------------------------------------------------------------
void CThread::Run()
{
	while (RUN == m_State )
	{
		//1. �½�ũ ó��
		EnterTaskSync();
		{
			TaskItor it = m_Tasks.begin();
			while (m_Tasks.end() != it)
			{
				CTask *pTask = *it;
				if (CTask::RR_END == pTask->Run())
				{
					// �½�ũ ����
					it = m_Tasks.erase(it);
					delete pTask;
				}
				else
				{
					++it;
				}
			}
		}
		LeaveTaskSync();

		//2. �޼��� ó��
		EnterMsgSync();
		{
			ExternalMsgItor it = m_ThreadMsgs.begin();
			while (m_ThreadMsgs.end() != it)
			{
				if (0 == it->rcvTaskId) // Thread���� �� �޼���
				{
					MessageProc(it->msg, it->wParam, it->lParam);
				}
				else
				{
					TaskItor t = find_if(m_Tasks.begin(), m_Tasks.end(), IsTask(it->rcvTaskId));
					if (m_Tasks.end() != t)
					{
						(*t)->MessageProc(it->msg, it->wParam, it->lParam);
					}
					else
					{
						// �������� ���� �޼��� error
					}
				}
				++it;
			}
			m_ThreadMsgs.clear();
		}
		LeaveMsgSync();

		Sleep(1);
	}
}
