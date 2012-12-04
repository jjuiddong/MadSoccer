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
	class CWindow : public CSyncNode
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

	public:
		void			Show(bool isShow, bool isApplyChild=true);
		void			Clear();
		void			Move(int elapsTime); // milli second
		virtual void	OnShow();
		virtual void	OnHide();

		// Getter/Setter
		int				GetId() const { return m_Id; }
		CDisplayObject*	GetDisplayObject() const { return m_pDispObj; }

		// overriding
		virtual void	Release() override;
		virtual bool	AddChild(CWindow *pChild);
		virtual bool	RemoveChild(CWindow *pChild);

		virtual void	MessagePorc(UINT message, WPARAM wParam, LPARAM lParam);
		virtual void	OnMoveHandling() {}
		virtual void	OnShowHandling() {}
		virtual void	OnHideHandling() {}

	};

}
