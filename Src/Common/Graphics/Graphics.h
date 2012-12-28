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
#include "LIb/SyncNode.h"
#include "LIb/Window.h"
#include "LIb/DisplayObject.h"
#include "LIb/DummyDisplayObject.h"



namespace graphics
{
	class CWindow;

	void		Init(HWND hWnd);		// 엔진 초기화
	void		ShutDown();				// 엔진 종료
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

	// Graphic Global Function
	int			GenerateWindowId();

	// Rendering
	// 멀티쓰레딩을 감안해서 구현했다.
	void		SetIndentityTransform();
	void		SetTransform( const Matrix44 &mat );
	const Matrix44& GetTransform();
	void		MultiplyTransform( const Matrix44 &mat );
	void		Render( const Vector3 *pVtxBuff, const int VtxSize, const Short2 *pIdxBuff, const int IdxSize );
	void		RenderGDI( HDC hdc, const Vector3 *pVtxBuff, const int VtxSize, const Short2 *pIdxBuff, const int IdxSize );


};

#endif __GRAPHICS_H__
