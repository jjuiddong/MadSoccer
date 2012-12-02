
#include "stdafx.h"
#include "Graphics.h"
#include "Window.h"
#include <mmsystem.h>
#include <process.h> 
#include "Renderer.h"



using namespace graphics;

namespace graphics
{
	class CWindow;

	common::CThread m_Thread;

	STATE		m_State = INIT;				// ���� ����
	CWindow		*m_pRootWindow = NULL;		// �ֻ��� ������
 	int			m_Fps = 30;					// �ʴ� ��� �����Ӽ�
	int			m_StartT = 0;				// ���α׷��� ���۵� �ð�
 	int			m_CurTime = 0;				// ���α׷��� ���۵� ���ĺ��� �帥 �ð� (millisecond ����)

}


//------------------------------------------------------------------------
// �׷��� ���� �ʱ�ȭ
//------------------------------------------------------------------------
void graphics::Init(HWND hWnd)
{
	m_Thread.AddTask( new CRenderer() );
	m_Thread.Start();

	m_State = RENDER;
	m_StartT = timeGetTime();

}


//------------------------------------------------------------------------
// ���� ����
//------------------------------------------------------------------------
void graphics::ShutDown()
{
	m_State = SHUTDOWN;
	m_Thread.Send2ThreadMessage( TASK_RENDERER, MSG_RENDERER_SETSTATE, SHUTDOWN, 0 );

}


//------------------------------------------------------------------------
// ������ ���ν��� ó��
// �� �����Ӹ��� ȣ��Ǿ�� �Ǵ� �Լ���.
//------------------------------------------------------------------------
void graphics::Proc()
{
	const int curT = timeGetTime();
	const int elapseT = curT - m_CurTime;
	m_CurTime = curT - m_StartT;

	if (RENDER == m_State)
	{
		if (m_pRootWindow)
			m_pRootWindow->Move(elapseT);
	}
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void graphics::WindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

}


//------------------------------------------------------------------------
// ��Ʈ ������ ����
// ���α׷��� ���۵ǰ� ó���� �ѹ��� ȣ��Ǿ�� �Ѵ�. �߰��� �ٽ� ȣ��Ǹ� 
// ������ ���� �� �ִ�.
//------------------------------------------------------------------------
void graphics::SetRootWindow(CWindow *pWnd)
{
	m_pRootWindow = pWnd;
	m_Thread.Send2ThreadMessage( TASK_RENDERER, MSG_RENDERER_SETROOTWINDOW, (WPARAM)pWnd, 0 );
}
graphics::CWindow* graphics::GetRootWindow()
{
	return m_pRootWindow;
}


//------------------------------------------------------------------------
// ������ ����
//------------------------------------------------------------------------
void graphics::SetFPS(int fps)
{
	m_Fps = fps;
	m_Thread.Send2ThreadMessage( TASK_RENDERER, MSG_RENDERER_SETFPS, fps, 0 );
}
int	graphics::GetFPS()
{
	return m_Fps;
}

//------------------------------------------------------------------------
// ���α׷��� ���۵� ���� �帥 �ð��� �����Ѵ�.
//------------------------------------------------------------------------
int graphics::GetCurTime()
{
	return m_CurTime;
}

//------------------------------------------------------------------------
// ���� ����
//------------------------------------------------------------------------
void graphics::SetState(STATE state)
{
	m_State = state;
	m_Thread.Send2ThreadMessage( TASK_RENDERER, MSG_RENDERER_SETSTATE, state, 0 );
}
STATE graphics::GetState()
{
	return m_State;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void graphics::Render( Vector3 *pVtxBuff, Vector3 *pIdBuff )
{
	// �����ؾ� ��
}

