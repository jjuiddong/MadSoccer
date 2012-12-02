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

	protected:
		STATE				m_State;
		CWindow				*m_pRootWindow;
		int					m_CurTime;				// ���α׷��� ���۵� ���ĺ��� �帥 �ð� (millisecond ����)
		int					m_StartT;				// ���α׷��� ����� �ð� (millisecond ����)
		int					m_Fps;					// �ʴ� ��� �����Ӽ�
		int					m_RenderTime;			// (1000/fps) �� millisecond�� ������ ��������� �����ϴ� ��
		int					m_IncT;					// �����Ǵ� �ð� �� (millisecond ����)

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
