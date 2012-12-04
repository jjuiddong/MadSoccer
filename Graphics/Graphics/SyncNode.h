//------------------------------------------------------------------------
// Name:    SyncNode.h
// Author:  jjuiddong
// Date:    2012-12-03
// 
// 멀티쓰레드 프로그래밍에서 쓰일 동기화 객체다.
// 다른 쓰레드끼리 접근하는 객체를 제거할 때 쓰인다.
//------------------------------------------------------------------------

#pragma once


namespace graphics
{
	class CSyncNode : public common::CInstance
	{
	protected:
		CSyncNode() : m_pParent(NULL) {}
		virtual ~CSyncNode() {}

	protected:
		CSyncNode		*m_pParent;
		SyncNodeList	m_Child;

	public:
		void				SetParent(CSyncNode *p) { m_pParent = p; }
		CSyncNode*			GetParent() { return m_pParent; }
		const SyncNodeList&	GetChild() { return m_Child; }

		// Override
		virtual void		Release() override;
		virtual bool		AddChild(CSyncNode *pChild);
		virtual bool		RemoveChild(CSyncNode *pChild);
		void				DeleteAll();

	};

}
