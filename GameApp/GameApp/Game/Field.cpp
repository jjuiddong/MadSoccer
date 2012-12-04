
#include "stdafx.h"
#include "Field.h"
#include "player.h"

using namespace game;


CField::CField() :
	graphics::CWindow(ID_FIELD, "Field")
{
	Vector3 vtx[] = {
		// Home
		// 운동장
		Vector3(0,  0,  0),		//0
		Vector3(70, 0,  0),		//1
		Vector3(70, 0, 105),	//2
		Vector3(0,  0, 105),	//3
		
		// 페널티 박스
		Vector3(15, 0, 0),		//4
		Vector3(55, 0, 0),		//5
		Vector3(55, 0, 20),		//6
		Vector3(15, 0, 20),		//7

		// 키퍼 박스
		Vector3(25, 0, 0),		//8
		Vector3(45, 0, 0),		//9
		Vector3(45, 0, 10),		//10
		Vector3(25, 0, 10),		//11

		// 중앙선
		Vector3(0, 0, 52.5f),	//12
		Vector3(70, 0, 52.5f),

		// Away
		// 페널티 박스
		Vector3(15, 0, 105),	//14
		Vector3(15, 0, 85),		//15
		Vector3(55, 0, 85),		//16
		Vector3(55, 0, 105),	//17

		// 키퍼 박스
		Vector3(25, 0, 105),	//18
		Vector3(25, 0, 95),		//19
		Vector3(45, 0, 95),		//20
		Vector3(45, 0, 105),	//21

	};
	const int vtxSize = sizeof(vtx)/sizeof(Vector3);

	Short2 idx[] = {
		Short2(0,4),
		Short2(4,5), Short2(5,6), Short2(6,7), Short2(7,4),
		Short2(9,10), Short2(10,11), Short2(11,8),
		Short2(5,1),
		Short2(1,2),
		Short2(0,3),
		Short2(12,13), // 중앙선

		Short2(3,14), 
		Short2(14,15), Short2(15,16), Short2(16,17), Short2(17,14),
		Short2(18,19), Short2(19,20), Short2(20,21),
		Short2(17,2),

		// 중앙원
		Short2(22,23), Short2(23,24),Short2(24,25),Short2(25,26),
		Short2(26,27), Short2(27,28),
	};
	const int idxSize = sizeof(idx) / sizeof(Short2);



	std::list<Vector3> vtxList;
	std::list<Short2> idxList;
	for (int i=0; i < vtxSize; ++i)
		vtxList.push_back( vtx[ i]);
	for (int i=0; i < idxSize; ++i)
		idxList.push_back( idx[ i]);

	// 중앙원
	const int firstIdx = vtxList.size();
	const float circleR = 9.15f;
	for (float i=0; i < MATH_PI*2.1f; i+=0.1f)
		vtxList.push_back( Vector3(35.f+cos(i)*circleR, 0, 52.5f+sin(i)*circleR) );
	const int endIdx = vtxList.size();
	for (int i=0; i < endIdx-firstIdx-1; ++i)
		idxList.push_back(Short2(firstIdx+i, firstIdx+i+1));
	
	m_pDispObj = new graphics::CDisplayObject("Field");
	m_pDispObj->Load(vtxList, idxList);

	Matrix44 matTM;
	matTM.SetIdentity();
	matTM.SetScale(Vector3(5,5,5));
	m_pDispObj->SetTM(matTM);


	AddChild( (graphics::CWindow*)new CPlayer() );

}

CField::~CField()
{

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CField::OnMoveHandling()
{


}

