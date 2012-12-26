//------------------------------------------------------------------------
// Name:    Reference.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// 인자로 넘어온 타입의 포인터를 멤버변수로 가진다.
//
// 다른 곳에서 할당된 메모리 포인터를 참조할 때 사용하는 클래스다.
// 함부로 메모리를 제거하는 것을 막기위해 만들어졌다.
//------------------------------------------------------------------------
#pragma once

namespace common
{
	template <class T>
	class ReferencePtr
	{
	public:
		ReferencePtr():m_p(NULL) {}
		ReferencePtr(T *p) : m_p(p) {}
	protected:
		T *m_p;
	public:
		T* operator->() const { return m_p; }
		bool operator!() const { return (m_p)? false : true; }
		T* operator=(T *p) { m_p = p; return m_p; }
	};
}
