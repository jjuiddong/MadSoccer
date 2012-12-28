//------------------------------------------------------------------------
// Name:    GameFramework.h
// Author:  jjuiddong
// Date:    2012-12-04
// 
// MadSoccer 게임 프레임 워크
//------------------------------------------------------------------------

#pragma once

#include "FrameWork.h"


namespace game
{
	class CGameFrameWork : public CFrameWork
	{
	public:
		CGameFrameWork();
		virtual ~CGameFrameWork() {}

	protected:

	public:
		virtual void		OnInitHandling() override;
		virtual void		OnProcHandling() override;
		virtual void		OnClearHandling() override;

	};

	// 프레임워크 인스턴스 선택
	CFrameWork*				CreateFrameWork() { return new CGameFrameWork(); }

}
