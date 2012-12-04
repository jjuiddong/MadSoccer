
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
//,	m_pParent(NULL)
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
	SyncNodeItor it = m_Child.begin();
	while (m_Child.end() != it)
	{
		CWindow *pwnd = (CWindow*)*it++;
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
	if (!CSyncNode::AddChild(pChild))
		return false;

	if (m_pDispObj)
		m_pDispObj->AddChild( pChild->GetDisplayObject() );
	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CWindow::RemoveChild(CWindow *pChild)
{
	if (!CSyncNode::RemoveChild(pChild))
		return false;

	if (m_pDispObj)
		m_pDispObj->RemoveChild( pChild->GetDisplayObject() );
	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CWindow::Clear()
{

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
		SyncNodeItor it = m_Child.begin();
		while (m_Child.end() != it)
		{
			CWindow *pwnd = (CWindow*)*it++;
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


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CWindow::Release()
{
	CSyncNode::Release();
	SAFE_RELEASE(m_pDispObj);

}

