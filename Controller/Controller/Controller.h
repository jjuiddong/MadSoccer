
// Controller.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CControllerApp:
// �� Ŭ������ ������ ���ؼ��� Controller.cpp�� �����Ͻʽÿ�.
//

class CControllerApp : public CWinAppEx
{
public:
	CControllerApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CControllerApp theApp;