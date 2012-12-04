
#include "stdafx.h"
#include "FrameWork.h"
#include "TestWindow.h"

using namespace game;

CFrameWork::CFrameWork() :
	m_hWnd(NULL)
,	m_pRootWindow(NULL)
{

}

CFrameWork::~CFrameWork() 
{
	Clear();

}


//------------------------------------------------------------------------
// 클래스 초기화
//------------------------------------------------------------------------
void CFrameWork::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	m_pRootWindow = new CTestWindow();

	graphics::Init( hWnd );
	graphics::SetRootWindow(m_pRootWindow);

}


//------------------------------------------------------------------------
// 매프래임마다 호출되는 함수
//------------------------------------------------------------------------
void CFrameWork::Proc()
{
	graphics::Proc();

}


//------------------------------------------------------------------------
// 프로그램 종료
//------------------------------------------------------------------------
void CFrameWork::ShutDown()
{
	Clear();

	graphics::ShutDown(); // 가장 나중에 호출되어야 한다.

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CFrameWork::Clear()
{
	SAFE_RELEASE(m_pRootWindow);

}
