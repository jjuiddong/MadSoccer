
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
		return false; // 이미 존재한다면 실패

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
		return false; // 없다면 실패

	pChild->SetParent(NULL);
	m_Child.remove(pChild);
	//delete pChild;
	return true;
}


//------------------------------------------------------------------------
// 연결된 모든 노드들의 메모리를 제거한다.
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

