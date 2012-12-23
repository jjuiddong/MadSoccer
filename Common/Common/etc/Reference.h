//------------------------------------------------------------------------
// Name:    Reference.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// ���ڷ� �Ѿ�� Ÿ���� �����͸� ��������� ������.
//
// �ٸ� ������ �Ҵ�� �޸� �����͸� ������ �� ����ϴ� Ŭ������.
// �Ժη� �޸𸮸� �����ϴ� ���� �������� ���������.
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
		T* operator->() { return m_p; }
		bool operator!() { return (m_p)? true : false; }
		T* operator=(T *p) { m_p = p; return m_p; }
	};
}
