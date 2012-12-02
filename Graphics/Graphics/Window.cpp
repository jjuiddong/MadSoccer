
#include "stdafx.h"
#include "Window.h"
#include "DisplayObject.h"


using namespace graphics;

CWindow::CWindow(int id=0, std::string name="UnTitle") :
	m_Id(id)
,	m_Name(name)
,	m_pDispObj(NULL)
,	m_IsVisible(true)
,	m_OldVisibleFlag(true)
{

}

CWindow::~CWindow() 
{
	Clear();
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
// void CWindow::Render()
// {
// 	if (m_IsVisible != m_OldVisibleFlag)
// 	{
// 		if (m_IsVisible)	OnShow();
// 		else				OnHide();
// 	}
// 
// 	// 자신을 먼저 출력하고 자식들을 출력한다.
// 	if (m_pDispObj)
// 		m_pDispObj->Render();
// 
// 	WindowItor it = m_Child.begin();
// 	while (m_Child.end() != it)
// 	{
// 		CWindow *pwnd = *it++;
// 		pwnd->Render();
// 	}
// 
// }


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CWindow::Move(int elapsTime )
{
	WindowItor it = m_Child.begin();
	while (m_Child.end() != it)
	{
		CWindow *pwnd = *it++;
		pwnd->Move(elapsTime);
	}

	if (m_pDispObj)
		m_pDispObj->Animation(elapsTime);
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CWindow::AddChild(CWindow *pChild)
{
	WindowItor it = find(m_Child.begin(), m_Child.end(), pChild);
	if (m_Child.end() != it)
		return false; // 이미 존재한다면 실패

	m_Child.push_back(pChild);
	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CWindow::RemoveChild(CWindow *pChild)
{
	WindowItor it = find(m_Child.begin(), m_Child.end(), pChild);
	if (m_Child.end() == it)
		return false; // 없다면 실패

	m_Child.remove(pChild);
	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CWindow::Clear()
{
	WindowItor it = m_Child.begin();
	while (m_Child.end() != it)
	{
		CWindow *pwnd = *it++;
		delete pwnd;
	}
	m_Child.clear();
}


//------------------------------------------------------------------------
// 윈도우 메세지처리, 오버라이딩해서 구현한다.
//------------------------------------------------------------------------
void CWindow::MessagePorc(UINT message, WPARAM wParam, LPARAM lParam)
{

}


//------------------------------------------------------------------------
// 화면에 출력할지 여부를 설정한다.
//------------------------------------------------------------------------
void CWindow::Show(bool isShow, bool isApplyChild) // applyChild=true
{
	m_OldVisibleFlag = m_IsVisible;
	m_IsVisible = isShow;
	// OnShow/OnHide 호출여부는 render() 에서 판단한다.

	if (isApplyChild)
	{
		WindowItor it = m_Child.begin();
		while (m_Child.end() != it)
		{
			CWindow *pwnd = *it++;
			pwnd->Show(isShow, isApplyChild);
		}
	}
}


//------------------------------------------------------------------------
// 윈도우가 화면에 표시될 때, 호출된다.
//------------------------------------------------------------------------
void CWindow::OnShow()
{
	m_IsVisible = true;
	m_OldVisibleFlag = true;
	OnShowHandling();
}


//------------------------------------------------------------------------
// 윈도우가 화면에 가려질 때, 호출된다.
//------------------------------------------------------------------------
void CWindow::OnHide()
{
	m_IsVisible = false;
	m_OldVisibleFlag = false;
	OnHideHandling();
}

