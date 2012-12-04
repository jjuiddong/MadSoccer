
#include "stdafx.h"
#include "SyncNode.h"

using namespace graphics;


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CSyncNode::Release()
{
	graphics::ReleaseSyncInstance(this);
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CSyncNode::AddChild(CSyncNode *pChild)
{
	SyncNodeItor it = find(m_Child.begin(), m_Child.end(), pChild);
	if (m_Child.end() != it)
		return false; // �̹� �����Ѵٸ� ����

	pChild->SetParent(this);
	m_Child.push_back(pChild);
	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CSyncNode::RemoveChild(CSyncNode *pChild)
{
	SyncNodeItor it = find(m_Child.begin(), m_Child.end(), pChild);
	if (m_Child.end() == it)
		return false; // ���ٸ� ����

	pChild->SetParent(NULL);
	m_Child.remove(pChild);
	//delete pChild;
	return true;
}


//------------------------------------------------------------------------
// ����� ��� ������ �޸𸮸� �����Ѵ�.
//------------------------------------------------------------------------
void CSyncNode::DeleteAll()
{
	SyncNodeItor it = m_Child.begin();
	while (m_Child.end() != it)
	{
		CSyncNode *p = *it++;
		p->DeleteAll();
	}
	m_Child.clear();
	delete this;
}

