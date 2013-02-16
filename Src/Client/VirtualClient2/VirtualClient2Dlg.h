//------------------------------------------------------------------------
// Name:    VirtualClient2Dlg.h
// Author:  jjuiddong
// Date:    1/3/2013
// 
// VirtualClient2Dlg.h : 헤더 파일
//------------------------------------------------------------------------
#pragma once
#include "afxcmn.h"
#include "ProtocolTree.h"

class CDlgProperty;
class CDlgConsole;
class CVirtualClient2Dlg : public CDialog
{
public:
	CVirtualClient2Dlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
