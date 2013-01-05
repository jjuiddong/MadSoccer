//------------------------------------------------------------------------
// Name:    DlgConsole.h
// Author:  jjuiddong
// Date:    1/3/2013
// 
// �ܼ� ���̾�α�
//------------------------------------------------------------------------
#pragma once


class CDlgConsole : public CDialog
{
	DECLARE_DYNAMIC(CDlgConsole)

public:
	CDlgConsole(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgConsole();
	enum { IDD = IDD_DLG_CONSOLE };

protected:
	CListBox		m_ListBox;

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
};
