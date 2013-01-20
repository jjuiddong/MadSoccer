
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CViewTree â�Դϴ�.

class CViewTree : public CTreeCtrl
{
// �����Դϴ�.
public:
	CViewTree();
	virtual ~CViewTree();

// �������Դϴ�.
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// �����Դϴ�.
public:
	HTREEITEM GetSymbolTreeItem(HTREEITEM hItem);
	HTREEITEM GetSelectSymbolTreeItem();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
};
