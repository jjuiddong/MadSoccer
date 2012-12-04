//------------------------------------------------------------------------
// Name:    Graphics.h
// Author:  jjuiddong
// Date:    2012-12-01
// 
// �׷��� ����
//------------------------------------------------------------------------

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "GraphicsDef.h"
#include "SyncNode.h"
#include "Window.h"
#include "DisplayObject.h"
#include "DummyDisplayObject.h"



namespace graphics
{
	class CWindow;

	void		Init(HWND hWnd);		// ���� �ʱ�ȭ
	void		ShutDown();				// ���� ����
	void		Proc();
	void		WindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void		ReleaseSyncInstance(CSyncNode *pNode);


	// Data Setting
 	void		SetState(STATE state);
 	STATE		GetState();
	void		SetRootWindow(CWindow *pWnd);
	CWindow*	GetRootWindow();
	void		SetFPS(int fps);
	int			GetFPS();
	int			GetCurTime();


	// Rendering
	// ��Ƽ�������� �����ؼ� �����ߴ�.
	void		Render( const Vector3 *pVtxBuff, const int VtxSize, const Short2 *pIdxBuff, const int IdxSize );
	void		RenderGDI( HDC hdc, const Vector3 *pVtxBuff, const int VtxSize, const Short2 *pIdxBuff, const int IdxSize );


};

#endif __GRAPHICS_H__
