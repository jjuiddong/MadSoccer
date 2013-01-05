// dlg/TreeCtrlBase.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TreeCtrlBase.h"


// CTreeCtrlBase

CTreeCtrlBase::CTreeCtrlBase(TYPE type) :
m_Type(type)
{

}

CTreeCtrlBase::~CTreeCtrlBase()
{
}


BEGIN_MESSAGE_MAP(CTreeCtrlBase, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CTreeCtrlBase::OnTvnSelchanged)
END_MESSAGE_MAP()



// CTreeCtrlBase 메시지 처리기입니다.


//------------------------------------------------------------------------------
/**
@brief 트리 노드 추가
*/
//------------------------------------------------------------------------------
HTREEITEM CTreeCtrlBase::InsertTree(HTREEITEM hParent, const TCHAR *nodeName, void *pCorrespondDlg)
{
	TVINSERTSTRUCT info;
	info.hParent = hParent;
	info.item.pszText = (LPWSTR)nodeName;
	info.item.mask = TVIF_PARAM | TVIF_TEXT;
	info.item.lParam = (LPARAM)pCorrespondDlg;
	HTREEITEM item = InsertItem(&info);	
	return item;
}


//------------------------------------------------------------------------------
/**
@brief 
*/
//------------------------------------------------------------------------------
void CTreeCtrlBase::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	CString str = GetItemText(pNMTreeView->itemOld.hItem);

	if (TREE_WINDOW == m_Type)
	{
		CDialog *pOldDlg = (CDialog*)pNMTreeView->itemOld.lParam;
		if (pOldDlg)
			pOldDlg->ShowWindow(SW_HIDE);
	}

	*pResult = 0;
}
