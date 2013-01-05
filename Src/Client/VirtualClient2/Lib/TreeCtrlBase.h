#pragma once


// CTreeCtrlBase

class CTreeCtrlBase : public CTreeCtrl
{
public:
	enum TYPE
	{
		TREE_WINDOW,	// 노드를 클릭하면 lparam에 저장된 윈도우를 출력한다.
		TREE_ITEM,		// 노드를 클릭해도 아무 변화가 없다.
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
