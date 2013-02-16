
// VirtualClient2.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "VirtualClient2.h"
#include "VirtualClient2Dlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVirtualClient2App

BEGIN_MESSAGE_MAP(CVirtualClient2App, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CVirtualClient2App ����

CVirtualClient2App::CVirtualClient2App()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CVirtualClient2App ��ü�Դϴ�.

CVirtualClient2App theApp;


// CVirtualClient2App �ʱ�ȭ

BOOL CVirtualClient2App::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�.
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));

	common::dump::InstallSelfMiniDump();

	CVirtualClient2Dlg *pdlg = new CVirtualClient2Dlg();
	m_pMainWnd = pdlg;
	pdlg->Create(CVirtualClient2Dlg::IDD, NULL);
	pdlg->ShowWindow(SW_SHOW);

	bool bDoingBackgroundProcessing = true;
	while ( bDoingBackgroundProcessing ) 
	{ 
		MSG msg;
		while ( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) 
		{ 
			if ( !PumpMessage( ) ) 
			{ 
				bDoingBackgroundProcessing = FALSE; 
				::PostQuitMessage(0); 
				break; 
			} 
		} 
		// let MFC do its idle processing
		LONG lIdle = 0;
		while ( AfxGetApp()->OnIdle(lIdle++ ) )
			;  
		// Perform some background processing here 
		// using another call to OnIdle
		CVClient::Get()->Proc();
		Sleep(0);
	}

	pdlg->DestroyWindow();
	delete pdlg;

	return FALSE;
}

DlgConsolePtr CVirtualClient2App::GetConsole()
{
	return ((CVirtualClient2Dlg*)m_pMainWnd)->GetConsole();
}
DlgPropertyPtr CVirtualClient2App::GetProperty()
{
	return ((CVirtualClient2Dlg*)m_pMainWnd)->GetProperty();
}
DlgTreePtr CVirtualClient2App::GetTree()
{
	return ((CVirtualClient2Dlg*)m_pMainWnd)->GetTree();
}

