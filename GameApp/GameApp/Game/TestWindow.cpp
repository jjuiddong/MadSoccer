
#include "stdafx.h"
#include "testWindow.h"
#include "Field.h"

using namespace game;


CTestWindow::CTestWindow() :
	graphics::CWindow(ID_TESTWINDOW, "testWindow")
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
void CTestWindow::MessagePorc(UINT message, WPARAM wParam, LPARAM lParam)
{

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CTestWindow::OnMoveHandling()
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

