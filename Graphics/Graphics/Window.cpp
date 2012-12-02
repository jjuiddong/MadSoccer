
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
// 	// �ڽ��� ���� ����ϰ� �ڽĵ��� ����Ѵ�.
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
		return false; // �̹� �����Ѵٸ� ����

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
		return false; // ���ٸ� ����

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
// ������ �޼���ó��, �������̵��ؼ� �����Ѵ�.
//------------------------------------------------------------------------
void CWindow::MessagePorc(UINT message, WPARAM wParam, LPARAM lParam)
{

}


//------------------------------------------------------------------------
// ȭ�鿡 ������� ���θ� �����Ѵ�.
//------------------------------------------------------------------------
void CWindow::Show(bool isShow, bool isApplyChild) // applyChild=true
{
	m_OldVisibleFlag = m_IsVisible;
	m_IsVisible = isShow;
	// OnShow/OnHide ȣ�⿩�δ� render() ���� �Ǵ��Ѵ�.

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
// �����찡 ȭ�鿡 ǥ�õ� ��, ȣ��ȴ�.
//------------------------------------------------------------------------
void CWindow::OnShow()
{
	m_IsVisible = true;
	m_OldVisibleFlag = true;
	OnShowHandling();
}


//------------------------------------------------------------------------
// �����찡 ȭ�鿡 ������ ��, ȣ��ȴ�.
//------------------------------------------------------------------------
void CWindow::OnHide()
{
	m_IsVisible = false;
	m_OldVisibleFlag = false;
	OnHideHandling();
}

