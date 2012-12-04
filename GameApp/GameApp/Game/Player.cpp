
#include "stdafx.h"
#include "Player.h"


using namespace game;

CPlayer::CPlayer() :
	graphics::CWindow(ID_PLAYER, "Player")
{
	graphics::VertexList vtxList;
	graphics::IndexList idxList;
	const float circleR = 1.f;
	for (float i=0; i < MATH_PI*2.1f; i+=0.1f)
		vtxList.push_back( Vector3(cos(i)*circleR, 0, sin(i)*circleR) );

	const int vtxSize = vtxList.size();
	for (int i=0; i < vtxSize-1; ++i)
		idxList.push_back( Short2(i, i+1) );

	m_pDispObj = new graphics::CDisplayObject("Player");
	m_pDispObj->Load(vtxList, idxList);

	Matrix44 matTM;
	matTM.SetIdentity();
	matTM.SetScale(Vector3(5,5,5));
	m_pDispObj->SetTM(matTM);
	m_pDispObj->SetPos( Vector3(35,0,50) );

}

CPlayer::~CPlayer()
{

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CPlayer::OnMoveHandling()
{

}
