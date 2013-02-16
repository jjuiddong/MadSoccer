
// VirtualClient2.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
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


// CVirtualClient2App 생성

CVirtualClient2App::CVirtualClient2App()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CVirtualClient2App 개체입니다.

CVirtualClient2App theApp;


// CVirtualClient2App 초기화

BOOL CVirtualClient2App::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));

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

