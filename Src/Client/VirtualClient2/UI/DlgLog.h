//------------------------------------------------------------------------
// Name:    DlgLog.h
// Author:  jjuiddong
// Date:    1/3/2013
// 
// �ܼ� ���̾�α�
//------------------------------------------------------------------------
#pragma once

class CDlgLog : public CDialog
{
	DECLARE_DYNAMIC(CDlgLog)

public:
	CDlgLog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgLog();
	enum { 
		IDD = IDD_DLG_CONSOLE,
		ID_LISTBOX,
		ID_BTN_CLEAR,
	};

protected:
	CListBox	m_ListBox;
	CBrush		m_brush;
	CButton	m_BtnClear;

public:
	void			AddString(const std::string &str);

protected:
	void			CreateHorizontalScroll(const std::string &str);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedClear();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
