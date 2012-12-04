//------------------------------------------------------------------------
// Name:    FrameWork.h
// Author:  jjuiddong
// Date:    2012-12-03
// 
// 게임 프레임워크 클래스다.
//------------------------------------------------------------------------

#pragma once


namespace game
{
	class CFrameWork
	{
	public:
		CFrameWork();
		virtual ~CFrameWork();

	protected:
		graphics::CWindow	*m_pRootWindow;
		HWND				m_hWnd;

	public:
		void	Init(HWND hWnd);
		void	ShutDown();
		void	Proc();
		void	Clear();

	protected:

	};

}
