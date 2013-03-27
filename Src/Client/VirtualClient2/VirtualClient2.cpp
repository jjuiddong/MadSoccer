
// VirtualClient2.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "VirtualClient2.h"
#include "UI/VirtualClient2Dlg.h"
#include "Control/ConfigParser.h"

#include "wxMemMonitorLib/wxMemMonitor.h"
MEMORYMONITOR_INNER_PROCESS();


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CVirtualClient2App, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CVirtualClient2App::CVirtualClient2App()
{
}

CVirtualClient2App theApp;

BOOL CVirtualClient2App::InitInstance()
{
	//_CrtSetBreakAlloc(1410); 
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));

	common::dump::InstallSelfMiniDump();
	memmonitor::Init(memmonitor::INNER_PROCESS, m_hInstance, "virtualclient2_monitor.json" );

	CVirtualClient2Dlg *pdlg = new CVirtualClient2Dlg();
	m_pMainWnd = pdlg;
	pdlg->Create(CVirtualClient2Dlg::IDD, NULL);

	config::RepositionWindow();
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

	CVClient::Release();
	network::Clear();
	config::SaveWindowPosition();
	memmonitor::Cleanup();

	pdlg->DestroyWindow();
	delete pdlg;

	return FALSE;
}

DlgConsolePtr CVirtualClient2App::GetLogWindow()
{
	return ((CVirtualClient2Dlg*)m_pMainWnd)->GetLogWindow();
}
DlgPropertyPtr CVirtualClient2App::GetProperty()
{
	return ((CVirtualClient2Dlg*)m_pMainWnd)->GetProperty();
}
DlgTreePtr CVirtualClient2App::GetTree()
{
	return ((CVirtualClient2Dlg*)m_pMainWnd)->GetTree();
}
DlgVClientPtr CVirtualClient2App::GetMainDlg()
{
	return ((CVirtualClient2Dlg*)m_pMainWnd);
}
