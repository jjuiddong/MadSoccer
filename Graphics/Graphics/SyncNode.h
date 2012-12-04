//------------------------------------------------------------------------
// Name:    SyncNode.h
// Author:  jjuiddong
// Date:    2012-12-03
// 
// ��Ƽ������ ���α׷��ֿ��� ���� ����ȭ ��ü��.
// �ٸ� �����峢�� �����ϴ� ��ü�� ������ �� ���δ�.
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
		typedef std::list<CSyncNode*> SyncNodeList;
		typedef SyncNodeList::iterator SyncNodeItor;

		CSyncNode		*m_pParent;
		SyncNodeList	m_Child;

	public:
		void			SetParent(CSyncNode *p) { m_pParent = p; }
		CSyncNode*		GetParent() { return m_pParent; }

		// Override
		virtual void	Release() override;
		virtual bool	AddChild(CSyncNode *pChild);
		virtual bool	RemoveChild(CSyncNode *pChild);
		void			DeleteAll();

	};

}
