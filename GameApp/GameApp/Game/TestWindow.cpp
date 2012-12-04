
#include "stdafx.h"
#include "testWindow.h"
#include "Field.h"

using namespace game;


CTestWindow::CTestWindow() :
	graphics::CWindow(0, "testWindow")
{
	m_pDispObj = new graphics::CDummyDisplayObject();
	AddChild( new CField() );

}

CTestWindow::~CTestWindow() 
{

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CTestWindow::Move(int elapsTime)
{
	int a= 0;

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CTestWindow::MessagePorc(UINT message, WPARAM wParam, LPARAM lParam)
{

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CTestWindow::OnShowHandling()
{

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CTestWindow::OnHideHandling() 
{

}

