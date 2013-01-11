
#include "stdafx.h"
#include "ViewTree.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTree

CViewTree::CViewTree()
{
}

CViewTree::~CViewTree()
{
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGING, &CViewTree::OnTvnSelchanging)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree 메시지 처리기

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}

void CViewTree::OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CString str = GetItemText(pNMTreeView->itemNew.hItem); // 디버깅용
	CString strOld = GetItemText(pNMTreeView->itemOld.hItem); // 디버깅용

// 	{
// 		SItemInfo *pInfo = (SItemInfo*)pNMTreeView->itemOld.lParam;
// 		if (pInfo && pInfo->dlg)
// 			pInfo->dlg->ShowWindow(SW_HIDE);
// 	}
// 	{
// 		SItemInfo *pInfo = (SItemInfo*)pNMTreeView->itemNew.lParam;
// 		if (pInfo && pInfo->type == PROTOCOL)
// 		{
// 			CRect cr;
// 			GetClientRect(cr);
// 			pInfo->dlg->MoveWindow(cr.Width()+1, 0, cr.Width()-1, cr.Height());
// 
// 			pInfo->dlg->ShowWindow(SW_SHOW);
// 		}
// 	}

	CMainFrame *pFrm = (CMainFrame*)::AfxGetMainWnd();
	pFrm->GetPropertyWnd().UpdateProperty( str );

	*pResult = 0;
}
