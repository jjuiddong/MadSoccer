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
// 		std::string			m_TitleName;
// 		std::string			m_ClassName;
		TCHAR				m_TitleName[MAX_LOADSTRING];	// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
		TCHAR				m_ClassName[MAX_LOADSTRING];	// �⺻ â Ŭ���� �̸��Դϴ�.

		HACCEL				m_hAccelTable;

	public:
		void				Init();
		void				Proc();
		void				MainLoop();
		void				Clear();

		void				ShutDown();
		bool				CreateWindowApp(HINSTANCE hInstance, int nCmdShow);

 		void				SetRootWindow(graphics::CWindow *pWnd) { m_pRootWindow = pWnd; }
 		graphics::CWindow*	GetRootWindow() const { return m_pRootWindow; }
		void				SetTitleName( TCHAR *titleName ) { wcscpy_s(m_TitleName, titleName); }
		void				SetClassName( TCHAR *className ) { wcscpy_s(m_ClassName, className); }
// 		std::string			GetTitleName() const { return m_TitleName; }
// 		std::string			GetClassName() const { return m_ClassName; }

		// Overriding
		virtual void		OnInitHandling() {}
		virtual void		OnProcHandling() {}
		virtual void		OnClearHandling() {}

	protected:

	};

	// �����ӿ�ũ �ν��Ͻ��� �����Ѵ�. �ݵ�� �� �Լ��� �����ؾ� �Ѵ�.
	CFrameWork*				CreateFrameWork();
}
