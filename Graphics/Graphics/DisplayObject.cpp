
#include "stdafx.h"
#include "DisplayObject.h"


using namespace graphics;

CDisplayObject::CDisplayObject(std::string name) :
	m_Name(name)
,	m_pVertexBuffer(NULL)
,	m_pIndexBuffer(NULL)
,	m_VertexBufferSize(0)
,	m_IndexBufferSize(0)
{
	m_matTM.SetIdentity();

}

CDisplayObject::~CDisplayObject() 
{
	Clear();

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CDisplayObject::Load(Vector3 *pVtxBuff, int VtxSize, Short2 *pIdxBuff, int IdxSize )
{
	SAFE_DELETEA(m_pVertexBuffer);
	SAFE_DELETEA(m_pIndexBuffer);

	m_pVertexBuffer = new Vector3[ VtxSize];
	m_pIndexBuffer = new Short2[ IdxSize];
	memcpy(m_pVertexBuffer, pVtxBuff, sizeof(Vector3)*VtxSize);
	memcpy(m_pIndexBuffer, pIdxBuff, sizeof(Short2)*IdxSize);
	m_VertexBufferSize = VtxSize;
	m_IndexBufferSize = IdxSize;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CDisplayObject::Load(const std::list<Vector3> &vtxList, const std::list<Short2> &idxList)
{
	SAFE_DELETEA(m_pVertexBuffer);
	SAFE_DELETEA(m_pIndexBuffer);

	if (vtxList.empty() || idxList.empty())
		return;

	m_pVertexBuffer = new Vector3[ vtxList.size()];
	m_pIndexBuffer = new Short2[ idxList.size()];
	
	int cnt = 0;
	std::list<Vector3>::const_iterator vit = vtxList.begin();
	while (vtxList.end() != vit)
		m_pVertexBuffer[ cnt++] = *vit++;

	cnt = 0;
	std::list<Short2>::const_iterator iit = idxList.begin();
	while (idxList.end() != iit)
		m_pIndexBuffer[ cnt++] = *iit++;

	m_VertexBufferSize = vtxList.size();
	m_IndexBufferSize = idxList.size();
}


//------------------------------------------------------------------------
// 출력
//------------------------------------------------------------------------
void CDisplayObject::Render()
{
	graphics::Render(m_pVertexBuffer, m_VertexBufferSize, m_pIndexBuffer, m_IndexBufferSize);

	// 자식 출력
	SyncNodeItor it = m_Child.begin();
	while (m_Child.end() != it)
	{
		CDisplayObject *pobj = (CDisplayObject*)*it++;
		pobj->Render();
	}
}
void CDisplayObject::RenderGDI(HDC hdc)
{
	graphics::SetTransform(m_matTM);
	graphics::RenderGDI(hdc, m_pVertexBuffer, m_VertexBufferSize, m_pIndexBuffer, m_IndexBufferSize);

	// 자식 출력
	SyncNodeItor it = m_Child.begin();
	while (m_Child.end() != it)
	{
		CDisplayObject *pobj = (CDisplayObject*)*it++;
		pobj->RenderGDI(hdc);
	}
}


//------------------------------------------------------------------------
// milli second
//------------------------------------------------------------------------
void CDisplayObject::Animation(int elapseTime)
{
	// 에니메이션 처리

	// 자식 에니메이션 처리
	SyncNodeItor it = m_Child.begin();
	while (m_Child.end() != it)
	{
		CDisplayObject *pobj = (CDisplayObject*)*it++;
		pobj->Animation(elapseTime);
	}
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CDisplayObject::Clear()
{
	SAFE_DELETEA(m_pVertexBuffer);
	SAFE_DELETEA(m_pIndexBuffer);
	m_VertexBufferSize = 0;
	m_IndexBufferSize = 0;

}


//------------------------------------------------------------------------
// 위치 설정
//------------------------------------------------------------------------
void CDisplayObject::SetPos(const Vector3 &pos)
{
	m_matTM.Translate(pos);
}

