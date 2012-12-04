
#include "stdafx.h"
#include "Field.h"

using namespace game;


CField::CField() :
	graphics::CWindow(0, "Field")
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
	};
	const int idxSize = sizeof(idx) / sizeof(Short2);

	m_pDispObj = new graphics::CDisplayObject("Field");
	m_pDispObj->Load(vtx, vtxSize, idx, idxSize);

}

CField::~CField()
{

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CField::Move(int elapsTime)
{

}
