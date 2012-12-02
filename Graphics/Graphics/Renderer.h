//------------------------------------------------------------------------
// Name:    Renderer.h
// Author:  jjuiddong
// Date:    2012-12-02
// 
// 멀티쓰레드 시스템
// 그래픽을 화면에 출력한다.
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

	protected:
		STATE				m_State;
		CWindow				*m_pRootWindow;
		int					m_CurTime;				// 프로그램이 시작된 이후부터 흐른 시간 (millisecond 단위)
		int					m_StartT;				// 프로그램이 실행된 시간 (millisecond 단위)
		int					m_Fps;					// 초당 출력 프레임수
		int					m_RenderTime;			// (1000/fps) 몇 millisecond가 지나야 출력할지를 결정하는 값
		int					m_IncT;					// 증가되는 시간 값 (millisecond 단위)

	public:
		STATE				GetState() const { return m_State; }
		CWindow*			GetRootWindow() const { return m_pRootWindow; }
		int					GetFPS() const { return m_Fps; }

		// overriding
		virtual RUN_RESULT	Run() override;
		virtual void		MessageProc( int msg, WPARAM wParam, LPARAM lParam ) override;

	protected:
		void				SetState(STATE state) { m_State = state; }
		void				SetRootWindow(CWindow *pWnd) { m_pRootWindow = pWnd; }
		void				SetFPS(int fps);

	};

}
