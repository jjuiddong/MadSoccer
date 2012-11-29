
#include "stdafx.h"
#include "Window.h"
#include "DisplayObject.h"


using namespace graphics;

CWindow::CWindow(int id=0, std::string name="UnTitle") :
	m_Id(id)
,	m_Name(name)
,	m_pDispObj(NULL)
{

}

CWindow::~CWindow() 
{
	Clear();
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CWindow::Render()
{
	WindowItor it = m_Child.begin();
	while (m_Child.end() != it)
	{
		CWindow *pwnd = *it++;
		pwnd->Render();
	}

	if (m_pDispObj)
		m_pDispObj->Render();
}


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

