//------------------------------------------------------------------------
// Name:    Player.h
// Author:  jjuiddong
// Date:    2012-12-04
// 
// 축구게임의 선수를 정의한다.
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
