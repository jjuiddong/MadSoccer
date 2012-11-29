
#include "stdafx.h"
#include "DisplayObject.h"


using namespace graphics;

CDisplayObject::CDisplayObject() :
	m_pVertexBuffer(NULL)
,	m_pIndexBuffer(NULL)
{
	m_matTM.SetIdentity();

}

CDisplayObject::~CDisplayObject() 
{

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CDisplayObject::Load(Vector3 *pVtxBuff, int VtxSize, Vector3 *pIdxBuff, int IdxSize )
{
	SAFE_DELETEA(m_pVertexBuffer);
	SAFE_DELETEA(m_pIndexBuffer);

	m_pVertexBuffer = new Vector3[ VtxSize];
	m_pIndexBuffer = new Vector3[ IdxSize];
	memcpy(m_pVertexBuffer, pVtxBuff, sizeof(Vector3)*VtxSize);
	memcpy(m_pIndexBuffer, pIdxBuff, sizeof(Vector3)*IdxSize);

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CDisplayObject::Render()
{
	DispObjItor it = m_Child.begin();
	while (m_Child.end() != it)
	{
		CDisplayObject *pobj = *it++;
		pobj->Render();
	}

	graphics::Render(m_pVertexBuffer, m_pIndexBuffer);
}


//------------------------------------------------------------------------
// milli second
//------------------------------------------------------------------------
void CDisplayObject::Animation(int elapseTime)
{
	DispObjItor it = m_Child.begin();
	while (m_Child.end() != it)
	{
		CDisplayObject *pobj = *it++;
		pobj->Animation(elapseTime);
	}

	// ���ϸ��̼� ó��
}


//------------------------------------------------------------------------
// �ڽ� ��� �߰�
//------------------------------------------------------------------------
bool CDisplayObject::AddChild(CDisplayObject *pChild)
{
	DispObjItor it = find(m_Child.begin(), m_Child.end(), pChild);
	if (m_Child.end() != it)
		return false; // �̹� �����Ѵٸ� ����

	m_Child.push_back(pChild);
	return true;
}


//------------------------------------------------------------------------
// �ڽ� ��� ����
//------------------------------------------------------------------------
bool CDisplayObject::RemoveChild(CDisplayObject *pChild)
{
	DispObjItor it = find(m_Child.begin(), m_Child.end(), pChild);
	if (m_Child.end() == it)
		return false; // ���ٸ� ����

	m_Child.remove(pChild);
	delete pChild;
	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CDisplayObject::Clear()
{
	DispObjItor it = m_Child.begin();
	while (m_Child.end() != it)
	{
		CDisplayObject *pobj = *it++;
		delete pobj;
	}
	m_Child.clear();

	SAFE_DELETEA(m_pVertexBuffer);
	SAFE_DELETEA(m_pIndexBuffer);

}
