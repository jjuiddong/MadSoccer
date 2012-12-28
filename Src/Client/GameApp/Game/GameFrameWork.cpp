
#include "stdafx.h"
#include "GameFrameWork.h"
#include "TestWindow.h"


using namespace game;

CGameFrameWork::CGameFrameWork()
{ 

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CGameFrameWork::OnInitHandling()
{
	CFrameWork::SetRootWindow(new CTestWindow());
	graphics::SetRootWindow( GetRootWindow() );

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CGameFrameWork::OnProcHandling()
{

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CGameFrameWork::OnClearHandling()
{

}

