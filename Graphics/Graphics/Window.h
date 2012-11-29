//------------------------------------------------------------------------
// Name:    Window.h
// Author:  jjuiddong
// Date:    2012-11-29
// 
// ȭ�鿡 ǥ�õǴ� ��� ���� �� ��ü�� ���Ѵ�.
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

		WindowList		m_Child;

	public:
		virtual void	Render();
		virtual void	Move(int elapsTime); // milli second

		bool			AddChild(CWindow *pChild);
		bool			RemoveChild(CWindow *pChild);
		void			Clear();

	};

}
