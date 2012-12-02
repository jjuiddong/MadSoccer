
#include "stdafx.h"
#include "Renderer.h"
#include <mmsystem.h>
#include "window.h"
#include "DisplayObject.h"



using namespace graphics;
using namespace common;


CRenderer::CRenderer() : 
	CTask(TASK_RENDERER)
,	m_State(INIT)
,	m_pRootWindow(NULL)
,	m_CurTime(0)
,	m_Fps(30)
,	m_RenderTime(1000/30)
,	m_IncT(0)
{
	m_StartT = timeGetTime();


}

CRenderer::~CRenderer()
{

}


//------------------------------------------------------------------------
// �׷��� ���
//------------------------------------------------------------------------
CTask::RUN_RESULT CRenderer::Run() 
{ 
	if (SHUTDOWN == m_State)
		return CTask::RR_END;

	const int curT = timeGetTime();
	const int elapseT = curT - m_CurTime;
	m_CurTime = curT - m_StartT;
	m_IncT += elapseT;

	switch (m_State)
	{
	case INIT: break;
	case RENDER:
		{
			if (m_IncT > m_RenderTime ) // FPS��ŭ ����ϰ� �Ѵ�.
			{
				CWindow *pRootWindow = graphics::GetRootWindow();
				if (pRootWindow)
				{
					CDisplayObject *pDispObj = pRootWindow->GetDisplayObject();
					pDispObj->Animation(elapseT);
					pDispObj->Render();
				}
				m_IncT = 0; // ������ ��ŵ
			}
		}
		break;

	case PAUSE: break;
	case DISPLAYCHANGE: break;
	case SHUTDOWN: break;
	}

	return CTask::RR_CONTINUE; 
}


//------------------------------------------------------------------------
// �������� �� �޼����� ó���Ѵ�.
//------------------------------------------------------------------------
void CRenderer::MessageProc( int msg, WPARAM wParam, LPARAM lParam )
{
	switch (msg)
	{
	case MSG_RENDERER_SETSTATE: SetState((STATE)wParam); break;
	case MSG_RENDERER_SETFPS: SetFPS((int)wParam); break;
	case MSG_RENDERER_SETROOTWINDOW: SetRootWindow( (CWindow*)wParam ); break;
	}
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CRenderer::SetFPS(int fps)
{
	m_Fps = fps;
	m_RenderTime = 1000 / fps;
}
