
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

	STATE		m_State = INIT;				// 엔진 상태
	CWindow		*m_pRootWindow = NULL;		// 최상위 윈도우
 	int			m_Fps = 30;					// 초당 출력 프레임수
	int			m_StartTime = 0;				// 프로그램이 시작된 시간
 	int			m_CurTime = 0;				// 프로그램이 시작된 이후부터 흐른 시간 (millisecond 단위)
	int			m_OldTime = 0;				// 그 전 프레임이 실행된 시간 (millisecond 단위)

	// Rendering
	Matrix44	m_matTransform;
}


//------------------------------------------------------------------------
// 그래픽 엔진 초기화
//------------------------------------------------------------------------
void graphics::Init(HWND hWnd)
{
	m_State = RENDER;
	m_StartTime = timeGetTime();
	m_OldTime = timeGetTime();
	m_matTransform.SetIdentity();

	m_Thread.AddTask( new CRenderer() );
	m_Thread.Start();

	// 렌더러 실행
	m_Thread.Send2ThreadMessage( TASK_RENDERER, MSG_RENDERER_SETSTATE, RENDER, 0 );
	m_Thread.Send2ThreadMessage( TASK_RENDERER, MSG_RENDERER_SETHWND, (WPARAM)hWnd, 0 );

}


//------------------------------------------------------------------------
// 엔진 종료
//------------------------------------------------------------------------
void graphics::ShutDown()
{
	m_State = SHUTDOWN;
	m_Thread.Send2ThreadMessage( TASK_RENDERER, MSG_RENDERER_SETSTATE, SHUTDOWN, 0 );
	m_Thread.Terminate();
}


//------------------------------------------------------------------------
// 윈도우 프로시져 처리
// 매 프레임마다 호출되어야 되는 함수다.
//------------------------------------------------------------------------
void graphics::Proc()
{
	const int curT = timeGetTime();
	const int elapseT = curT - m_OldTime;
	m_CurTime = curT - m_StartTime;
	m_OldTime = curT;

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
// 루트 윈도우 설정
// 프로그램이 시작되고 처음에 한번만 호출되어야 한다. 중간에 다시 호출되면 
// 문제가 생길 수 있다.
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
// 프레임 설정
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
// 프로그램이 시작된 이후 흐른 시간을 리턴한다.
//------------------------------------------------------------------------
int graphics::GetCurTime()
{
	return m_CurTime;
}

//------------------------------------------------------------------------
// 상태 설정
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
void graphics::ReleaseSyncInstance(CSyncNode *pNode)
{
	m_Thread.Send2ThreadMessage( TASK_RENDERER, MSG_RENDERER_RELEASE, 0, (WPARAM)pNode);
}


//------------------------------------------------------------------------
// 중복되지 않는 WindowID를 리턴한다.
//------------------------------------------------------------------------
bool SearchWindowTree(graphics::CWindow *pWnd, const int id)
{
	if (!pWnd)
		return true;
	if (id == pWnd->GetId())
		return false;
	const SyncNodeList &child = pWnd->GetChild();
	SyncNodeCItor it = child.begin();
	while (child.end() != it)
	{
		if (!SearchWindowTree((CWindow*)*it, id))
			return false;
	}
	return true;
}
int graphics::GenerateWindowId()
{
	static int genId = 1000;
	while (!SearchWindowTree(m_pRootWindow, genId))
		genId++;
	return genId;
}


//------------------------------------------------------------------------
// DirectX로 출력한다.
//------------------------------------------------------------------------
void graphics::Render( const Vector3 *pVtxBuff, const int VtxSize, const Short2 *pIdxBuff, const int IdxSize )
{

}


//------------------------------------------------------------------------
// 변환 매트릭스 함수
//------------------------------------------------------------------------
void graphics::SetIndentityTransform()
{
	m_matTransform.SetIdentity();
}
void graphics::SetTransform( const Matrix44 &mat )
{
	m_matTransform = mat;
}
const Matrix44& graphics::GetTransform()
{
	return m_matTransform;
}
void graphics::MultiplyTransform( const Matrix44 &mat )
{
	m_matTransform *= mat;
}

//------------------------------------------------------------------------
// GDI로 출력한다.
//------------------------------------------------------------------------
void graphics::RenderGDI( HDC hdc, const Vector3 *pVtxBuff, const int VtxSize, const Short2 *pIdxBuff, const int IdxSize )
{
	for (int i=0; i < IdxSize; ++i)
	{
		const u_short idx1 = pIdxBuff[ i].x;
		const u_short idx2 = pIdxBuff[ i].y;

		Vector3 v1;
		MultipleMatrix(pVtxBuff[ idx1], m_matTransform, v1);
		Vector3 v2;
		MultipleMatrix(pVtxBuff[ idx2], m_matTransform, v2);

 		MoveToEx(hdc, (int)v1.x, (int)v1.z, NULL);
		LineTo(hdc, (int)v2.x, (int)v2.z);
	}
}


