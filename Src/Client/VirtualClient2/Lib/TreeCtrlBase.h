#pragma once


// CTreeCtrlBase

class CTreeCtrlBase : public CTreeCtrl
{
public:
	enum TYPE
	{
		TREE_WINDOW,	// ��带 Ŭ���ϸ� lparam�� ����� �����츦 ����Ѵ�.
		TREE_ITEM,		// ��带 Ŭ���ص� �ƹ� ��ȭ�� ����.
	};

	CTreeCtrlBase(TYPE type);
	virtual ~CTreeCtrlBase();

	TYPE		m_Type;
	HTREEITEM	InsertTree(HTREEITEM hParent, const TCHAR *nodeName, void  *pCorrespondDlg);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};
