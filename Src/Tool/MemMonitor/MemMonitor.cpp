
// MemMonitor.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "MemMonitor.h"
#include "MainFrm.h"

#include "MemMonitorDoc.h"
#include "MemMonitorView.h"
#include "Lib/DiaWrapper.h"
#include <typeinfo.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const std::string g_configFileName("config.json");

// CMemMonitorApp

BEGIN_MESSAGE_MAP(CMemMonitorApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CMemMonitorApp::OnAppAbout)
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ǥ�� �μ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CMemMonitorApp ����

CMemMonitorApp::CMemMonitorApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}

CMemMonitorApp::~CMemMonitorApp()
{
	dia::CDiaWrapper::Get()->Release();
	sharedmemory::Release();
}


// ������ CMemMonitorApp ��ü�Դϴ�.

CMemMonitorApp theApp;


// CMemMonitorApp �ʱ�ȭ

BOOL CMemMonitorApp::InitInstance()
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
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.
// 	InitContextMenuManager();
// 	InitKeyboardManager();
// 	InitTooltipManager();
// 	CMFCToolTipInfo ttParams;
// 	ttParams.m_bVislManagerTheme = TRUE;
// 	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
// 		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	if (!OpenConfigScript(g_configFileName))
		return FALSE;

	// ���� ���α׷��� ���� ���ø��� ����մϴ�. ���� ���ø���
	//  ����, ������ â �� �� ������ ���� ������ �մϴ�.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMemMonitorDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ������ â�Դϴ�.
		RUNTIME_CLASS(CMemMonitorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// â �ϳ��� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// ���̻簡 ���� ��쿡�� DragAcceptFiles�� ȣ���մϴ�.
	//  SDI ���� ���α׷������� ProcessShellCommand �Ŀ� �̷��� ȣ���� �߻��ؾ� �մϴ�.

	((CMainFrame*)AfxGetMainWnd())->OpenScript(  g_configFileName );

	return TRUE;
}


//------------------------------------------------------------------------
// Config ��ũ��Ʈ�� ��� �⺻ȯ���� �����.
// pdb  ������ ����, shared memory �� �����Ѵ�.
//------------------------------------------------------------------------
BOOL CMemMonitorApp::OpenConfigScript(const std::string &configFileName)
{
	try
	{
		// boost property tree
		using boost::property_tree::ptree;
		using std::string;
		ptree props;
		boost::property_tree::read_json(configFileName.c_str(), props);
		string pdbPath = props.get<string>("pdbPath");
		string shareMemoryName = props.get<string>("sharedmemoryname");

		// Pdb Load
		if (!dia::CDiaWrapper::Get()->Init(pdbPath))
		{
			CString msg;
			msg.Format( L"%s Pdb ������ �����ϴ�.", 
				common::string2wstring(pdbPath).c_str() );
			::AfxMessageBox( msg );
			return FALSE;
		}
		if (!sharedmemory::Init(shareMemoryName, sharedmemory::SHARED_CLIENT))
		{
			CString msg;
			msg.Format( L"%s  �̸��� �����޸𸮰� �����ϴ�.", 
				common::string2wstring(shareMemoryName).c_str() );
			::AfxMessageBox( msg );
			return FALSE;
		}
	}
	catch (std::exception &e)
	{
		CString msg = common::formatw("\"%s\" json script Err!! [%s]",  
			configFileName.c_str(), e.what()).c_str();
		::AfxMessageBox( msg );
		return FALSE;
	}
	return TRUE;
}



// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CMemMonitorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMemMonitorApp ����� ���� �ε�/���� �޼���

void CMemMonitorApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CMemMonitorApp::LoadCustomState()
{
}

void CMemMonitorApp::SaveCustomState()
{
}

// CMemMonitorApp �޽��� ó����



