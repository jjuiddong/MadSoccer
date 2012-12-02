//------------------------------------------------------------------------
// Name:    Window.h
// Author:  jjuiddong
// Date:    2012-11-29
// 
// 윈도우 메세지와 화면을 출력하는 역할을 한다.
//------------------------------------------------------------------------
#pragma once

namespace graphics
{
	class CDisplayObject;
	class CWindow
	{
	public:
		CWindow(int id, std::string name);
		virtual ~CWindow();

	protected:
		int				m_Id;
		std::string		m_Name;
		CDisplayObject	*m_pDispObj;
		bool			m_IsVisible;
		bool			m_OldVisibleFlag; // isVisible 변수가 바뀌기 전의 값을 저장한다.

		WindowList		m_Child;

	public:
		int				GetId() const { return m_Id; }
		void			Show(bool isShow, bool isApplyChild=true);
		bool			AddChild(CWindow *pChild);
		bool			RemoveChild(CWindow *pChild);
		CDisplayObject*	GetDisplayObject() const { return m_pDispObj; }
		void			Clear();
		virtual void	OnShow();
		virtual void	OnHide();

		// overriding
		//virtual void	Render();
		virtual void	Move(int elapsTime); // milli second
		virtual void	MessagePorc(UINT message, WPARAM wParam, LPARAM lParam);
		virtual void	OnShowHandling() {}
		virtual void	OnHideHandling() {}

	};

}
