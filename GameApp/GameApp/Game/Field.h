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
		virtual void	Move(int elapsTime) override;

	};

}
