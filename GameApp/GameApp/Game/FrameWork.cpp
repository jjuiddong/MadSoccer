
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
// Ŭ���� �ʱ�ȭ
//------------------------------------------------------------------------
void CFrameWork::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	m_pRootWindow = new CTestWindow();

	graphics::Init( hWnd );
	graphics::SetRootWindow(m_pRootWindow);

}


//------------------------------------------------------------------------
// �������Ӹ��� ȣ��Ǵ� �Լ�
//------------------------------------------------------------------------
void CFrameWork::Proc()
{
	graphics::Proc();

}


//------------------------------------------------------------------------
// ���α׷� ����
//------------------------------------------------------------------------
void CFrameWork::ShutDown()
{
	Clear();

	graphics::ShutDown(); // ���� ���߿� ȣ��Ǿ�� �Ѵ�.

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CFrameWork::Clear()
{
	SAFE_RELEASE(m_pRootWindow);

}
