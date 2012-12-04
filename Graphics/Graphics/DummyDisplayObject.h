//------------------------------------------------------------------------
// Name:    DummyDisplayObject.h
// Author:  jjuiddong
// Date:    2012-12-03
// 
// 화면에 출력되지는 않지만, 자식들을 묶을 수 있는 객체다.
//------------------------------------------------------------------------

#pragma once


namespace graphics
{
	class CDummyDisplayObject : public CDisplayObject
	{
	public:
		CDummyDisplayObject() {}
		virtual ~CDummyDisplayObject() {}

	};

}
