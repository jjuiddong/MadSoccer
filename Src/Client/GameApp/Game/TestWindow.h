//------------------------------------------------------------------------
// Name:    TestWindow.h
// Author:  jjuiddong
// Date:    2012-12-03
// 
// 
//------------------------------------------------------------------------

#pragma once


namespace game
{

	class CTestWindow : public graphics::CWindow
	{
	public:
		CTestWindow();
		virtual ~CTestWindow();

	protected:

	public:
		virtual void	MessagePorc(UINT message, WPARAM wParam, LPARAM lParam);
		virtual void	OnMoveHandling() override;
		virtual void	OnShowHandling() override;
		virtual void	OnHideHandling() override;

	};

}
