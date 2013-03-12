//------------------------------------------------------------------------
// Name:    Renderer.h
// Author:  jjuiddong
// Date:    2012-12-02
// 
// ��Ƽ������ �ý���
// �׷����� ȭ�鿡 ����Ѵ�.
//------------------------------------------------------------------------
#pragma once

namespace graphics
{
	class CWindow;
	class CRenderer : public common::CTask
	{
	public:
		CRenderer();
		virtual ~CRenderer();
	
		// overriding
		virtual RUN_RESULT	Run() override;
		virtual void		MessageProc( common::threadmsg::MSG msg, WPARAM wParam, LPARAM lParam, LPARAM added ) override;

		STATE				GetState() const;
		const CWindow*	GetRootWindow() const;
		int					GetFPS() const;
		HWND				GetHWnd() const;

	protected:
		void				SetState(STATE state) { m_State = state; }
		void				SetRootWindow(CWindow *pWnd) { m_pRootWindow = pWnd; }
		void				SetFPS(int fps);
		void				SetHWnd(HWND hwnd) { m_hWnd = hwnd; }

		void				RenderGDI(int elapsedTime);
		void				RenderDX(int elapsedTime);
		void				ReleasePtr( int type, WPARAM ptr);

	protected:
		STATE				m_State;
		CWindow			*m_pRootWindow;
		HWND				m_hWnd;
		int					m_CurTime;				// ���α׷��� ���۵� ���ĺ��� �帥 �ð� (millisecond ����)
		int					m_OldTime;				// �� �������� ��µ� �ð� (millisecond����)
		int					m_StartTime;			// ���α׷��� ����� �ð� (millisecond ����)
		int					m_Fps;					// �ʴ� ��� �����Ӽ�
		int					m_RenderTime;			// (1000/fps) �� millisecond�� ������ ��������� �����ϴ� ��
		int					m_IncT;					// �����Ǵ� �ð� �� (millisecond ����)

	};

	inline STATE	CRenderer::GetState() const { return m_State; }
	inline const CWindow*	 CRenderer::GetRootWindow() const { return m_pRootWindow; }
	inline int CRenderer::GetFPS() const { return m_Fps; }
	inline HWND CRenderer::GetHWnd() const { return m_hWnd; }

}
