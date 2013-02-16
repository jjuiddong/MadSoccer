
// VirtualClient2Dlg.cpp : ���� ����
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

	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}


//------------------------------------------------------------------------
// WM_CREATE �޼����� �������̵� �ؾ� WM_SIZE�� �����ؼ� ũ�⸦ ��Ȯ�ϰ�
// ���� �� �ִ�. OninitialDialog ���� DlgProperty�� �����ϸ� OnSize�� 
// ������ ���� �ʾ� �ڵ� �ߺ��� �߻��ϰ� �ȴ�.
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
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
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

	// ��ü ���̾�α� ũ���� 2/3��ŭ DlgProperty �� ���� �ϰ�,
	// 1/3�� TreeCtrl �� �����ϰ� �Ѵ�.
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
