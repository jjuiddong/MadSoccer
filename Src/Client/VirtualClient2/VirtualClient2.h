
// VirtualClient2.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CVirtualClient2App:
// �� Ŭ������ ������ ���ؼ��� VirtualClient2.cpp�� �����Ͻʽÿ�.
//

class CVirtualClient2App : public CWinAppEx
{
public:
	CVirtualClient2App();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CVirtualClient2App theApp;