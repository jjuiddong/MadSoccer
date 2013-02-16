//------------------------------------------------------------------------
// Name:    VirtualClient2Dlg.h
// Author:  jjuiddong
// Date:    1/3/2013
// 
// VirtualClient2Dlg.h : ��� ����
//------------------------------------------------------------------------
#pragma once
#include "afxcmn.h"
#include "ProtocolTree.h"

class CDlgProperty;
class CDlgConsole;
class CVirtualClient2Dlg : public CDialog
{
public:
	CVirtualClient2Dlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	virtual ~CVirtualClient2Dlg();
	enum { 
		IDD = IDD_VIRTUALCLIENT2_DIALOG ,
		ID_PROTOCOL_TREE,
	};

protected:
	CDlgConsole		*m_pDlgConsole;
	CProtocolTree		m_ProtocolTree;	

public:
	DlgConsolePtr		GetConsole() { return m_pDlgConsole; }
	DlgPropertyPtr		GetProperty() { return NULL; }
	DlgTreePtr			GetTree() { return &m_ProtocolTree; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
