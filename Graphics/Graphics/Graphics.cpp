
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
	int			m_StartTime = 0;				// ���α׷��� ���۵� �ð�
 	int			m_CurTime = 0;				// ���α׷��� ���۵� ���ĺ��� �帥 �ð� (millisecond ����)
	int			m_OldTime = 0;				// �� �� �������� ����� �ð� (millisecond ����)

	// Rendering
	Matrix44	m_matTransform;
}


//------------------------------------------------------------------------
// �׷��� ���� �ʱ�ȭ
//------------------------------------------------------------------------
void graphics::Init(HWND hWnd)
{
	m_State = RENDER;
	m_StartTime = timeGetTime();
	m_OldTime = timeGetTime();
	m_matTransform.SetIdentity();

	m_Thread.AddTask( new CRenderer() );
	m_Thread.Start();

	// ������ ����
	m_Thread.Send2ThreadMessage( TASK_RENDERER, MSG_RENDERER_SETSTATE, RENDER, 0 );
	m_Thread.Send2ThreadMessage( TASK_RENDERER, MSG_RENDERER_SETHWND, (WPARAM)hWnd, 0 );

}


//------------------------------------------------------------------------
// ���� ����
//------------------------------------------------------------------------
void graphics::ShutDown()
{
	m_State = SHUTDOWN;
	m_Thread.Send2ThreadMessage( TASK_RENDERER, MSG_RENDERER_SETSTATE, SHUTDOWN, 0 );
	m_Thread.Terminate();
}


//------------------------------------------------------------------------
// ������ ���ν��� ó��
// �� �����Ӹ��� ȣ��Ǿ�� �Ǵ� �Լ���.
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
void graphics::ReleaseSyncInstance(CSyncNode *pNode)
{
	m_Thread.Send2ThreadMessage( TASK_RENDERER, MSG_RENDERER_RELEASE, 0, (WPARAM)pNode);
}


//------------------------------------------------------------------------
// �ߺ����� �ʴ� WindowID�� �����Ѵ�.
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
// DirectX�� ����Ѵ�.
//------------------------------------------------------------------------
void graphics::Render( const Vector3 *pVtxBuff, const int VtxSize, const Short2 *pIdxBuff, const int IdxSize )
{

}


//------------------------------------------------------------------------
// ��ȯ ��Ʈ���� �Լ�
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
// GDI�� ����Ѵ�.
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


