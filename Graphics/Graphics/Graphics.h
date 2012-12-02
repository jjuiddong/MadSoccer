//------------------------------------------------------------------------
// Name:    Graphics.h
// Author:  jjuiddong
// Date:    2012-12-01
// 
// 그래픽 엔진
//------------------------------------------------------------------------

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "GraphicsDef.h"


namespace graphics
{
	class CWindow;

	void		Init(HWND hWnd);		// 엔진 초기화
	void		ShutDown();				// 엔진 종료
	void		Proc();
	void		WindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Data Setting
 	void		SetState(STATE state);
 	STATE		GetState();
	void		SetRootWindow(CWindow *pWnd);
	CWindow*	GetRootWindow();
	void		SetFPS(int fps);
	int			GetFPS();
	int			GetCurTime();

	// Rendering
	void		Render( Vector3 *pVtxBuff, Vector3 *pIdBuff );

};

#endif __GRAPHICS_H__
