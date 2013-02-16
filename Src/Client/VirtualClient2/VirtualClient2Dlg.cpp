
// VirtualClient2Dlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "VirtualClient2.h"
#include "VirtualClient2Dlg.h"
#include "DlgProperty.h"
#include "DlgConsole.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CVirtualClient2Dlg::CVirtualClient2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVirtualClient2Dlg::IDD, pParent)
,	m_pDlgConsole(NULL)
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CVirtualClient2Dlg::~CVirtualClient2Dlg()
{
	SAFE_DELETE(m_pDlgConsole);
	network::Clear();
	CVClient::Release();
}

void CVirtualClient2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVirtualClient2Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CVirtualClient2Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CVirtualClient2Dlg::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
END_MESSAGE_MAP()


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
BOOL CVirtualClient2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


//------------------------------------------------------------------------
// WM_CREATE 메세지를 오버라이딩 해야 WM_SIZE와 연계해서 크기를 정확하게
// 맞출 수 있다. OninitialDialog 에서 DlgProperty를 생성하면 OnSize와 
// 순서가 맞지 않아 코드 중복이 발생하게 된다.
//------------------------------------------------------------------------
int CVirtualClient2Dlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	std::locale::global(std::locale(""));

	m_pDlgConsole = new CDlgConsole(this);
	m_pDlgConsole->Create( CDlgConsole::IDD, this );
	m_pDlgConsole->ShowWindow(TRUE);

	m_ProtocolTree.Create(WS_VISIBLE | WS_CHILD |
		TVS_FULLROWSELECT | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS 
		, CRect(0,0,100,100), this, ID_PROTOCOL_TREE);
	m_ProtocolTree.SetBkColor(RGB(140,140,140));
	m_ProtocolTree.Init();

	network::Init(0);

	return 0;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CVirtualClient2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{

	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CVirtualClient2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
HCURSOR CVirtualClient2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CVirtualClient2Dlg::OnBnClickedOk()
{
//	OnOK();
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CVirtualClient2Dlg::OnBnClickedCancel()
{
	PostQuitMessage(0);

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CVirtualClient2Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// 전체 다이얼로그 크기의 2/3만큼 DlgProperty 가 차지 하고,
	// 1/3은 TreeCtrl 이 차지하게 한다.
	const int dlgPropW = (int)(cx * 0.3333f);
	const int treeCtrlW = (int)(cx * 0.3333f);
	const int consoleW = (int)(cx * 0.3333f);

	if (m_ProtocolTree.GetSafeHwnd())
		m_ProtocolTree.MoveWindow(CRect(0,0,treeCtrlW,cy));
	if (m_pDlgConsole)
		m_pDlgConsole->MoveWindow(CRect(treeCtrlW+dlgPropW, 0,treeCtrlW+dlgPropW+consoleW,cy));
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CVirtualClient2Dlg::OnDestroy()
{
	CDialog::OnDestroy();

}
