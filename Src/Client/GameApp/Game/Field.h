//------------------------------------------------------------------------
// Name:    Field.h
// Author:  jjuiddong
// Date:    2012-12-03
// 
// �౸���� �����ϴ� Ŭ������.
//------------------------------------------------------------------------

#pragma once


namespace game
{
	class CField : public graphics::CWindow
	{
	public:
		CField();
		virtual ~CField();

	protected:

	public:
		// Override
		virtual void	OnMoveHandling() override;
		virtual void	OnKeyProcHandling(UINT message, WPARAM wParam, LPARAM lParam) override;
	};

}
