//------------------------------------------------------------------------
// Name:    Player.h
// Author:  jjuiddong
// Date:    2012-12-04
// 
// �౸������ ������ �����Ѵ�.
//------------------------------------------------------------------------

#pragma once


namespace game
{
	class CPlayer : graphics::CWindow
	{
	public:
		CPlayer();
		virtual ~CPlayer();

	protected:

	public:
		virtual void	OnMoveHandling() override;

	};

}
