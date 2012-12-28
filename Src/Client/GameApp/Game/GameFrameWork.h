//------------------------------------------------------------------------
// Name:    GameFramework.h
// Author:  jjuiddong
// Date:    2012-12-04
// 
// MadSoccer ���� ������ ��ũ
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

	// �����ӿ�ũ �ν��Ͻ� ����
	CFrameWork*				CreateFrameWork() { return new CGameFrameWork(); }

}
