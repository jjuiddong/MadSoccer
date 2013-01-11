
// MemMonitorView.cpp : CMemMonitorView Ŭ������ ����
//

#include "stdafx.h"
#include "MemMonitor.h"
#include "MemMonitorDoc.h"
#include "MemMonitorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMemMonitorView

IMPLEMENT_DYNCREATE(CMemMonitorView, CView)

BEGIN_MESSAGE_MAP(CMemMonitorView, CView)
END_MESSAGE_MAP()

// CMemMonitorView ����/�Ҹ�

CMemMonitorView::CMemMonitorView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMemMonitorView::~CMemMonitorView()
{
}

BOOL CMemMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMemMonitorView �׸���

void CMemMonitorView::OnDraw(CDC* /*pDC*/)
{
	CMemMonitorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


void CMemMonitorView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMemMonitorView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CMemMonitorView ����

#ifdef _DEBUG
void CMemMonitorView::AssertValid() const
{
	CView::AssertValid();
}

void CMemMonitorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMemMonitorDoc* CMemMonitorView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMemMonitorDoc)));
	return (CMemMonitorDoc*)m_pDocument;
}
#endif //_DEBUG


// CMemMonitorView �޽��� ó����
