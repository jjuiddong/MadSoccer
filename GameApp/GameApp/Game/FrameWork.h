//------------------------------------------------------------------------
// Name:    FrameWork.h
// Author:  jjuiddong
// Date:    2012-12-03
// 
// ���� �����ӿ�ũ Ŭ������.
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
