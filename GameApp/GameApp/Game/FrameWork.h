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
	protected:
		CFrameWork();
		virtual ~CFrameWork();

	public:
		enum
		{
			MAX_LOADSTRING = 100,
		};

		enum STATE
		{
			INIT,
			RUN,
			SHUTDOWN,			
		};

	//-----------------------------------------
	// singleton
	protected:
		static CFrameWork	*m_pInstance;
	public:
		static CFrameWork*	Get();
	//-------------------------------------------

	protected:
		STATE				m_State;
		graphics::CWindow	*m_pRootWindow;
		HWND				m_hWnd;
		tstring				m_TitleName;	// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
		tstring				m_ClassName;	// �⺻ â Ŭ���� �̸��Դϴ�.
		HACCEL				m_hAccelTable;

	public:
		void				Init();
		void				ShutDown();
		void				Proc();
		void				MainLoop();
		void				Clear();

		virtual void		MessageProc( UINT message, WPARAM wParam, LPARAM lParam);
		bool				CreateWindowApp(HINSTANCE hInstance, int nCmdShow);

		// Setter/Getter
 		void				SetRootWindow(graphics::CWindow *pWnd) { m_pRootWindow = pWnd; }
 		graphics::CWindow*	GetRootWindow() const { return m_pRootWindow; }
		void				SetTitleName( TCHAR *titleName ) { m_TitleName= titleName; }
		void				SetClassName( TCHAR *className ) { m_ClassName = className; }
 		tstring				GetTitleName() const { return m_TitleName; }
 		tstring				GetClassName() const { return m_ClassName; }

		// Overriding
		virtual void		OnInitHandling() {}
		virtual void		OnProcHandling() {}
		virtual void		OnClearHandling() {}

	protected:

	};

	// �����ӿ�ũ �ν��Ͻ��� �����Ѵ�. �ݵ�� �� �Լ��� �����ؾ� �Ѵ�.
	CFrameWork*				CreateFrameWork();
}
