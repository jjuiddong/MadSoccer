
// MemMonitorView.h : CMemMonitorView Ŭ������ �������̽�
//


#pragma once


class CMemMonitorView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMemMonitorView();
	DECLARE_DYNCREATE(CMemMonitorView)

// Ư���Դϴ�.
public:
	CMemMonitorDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// �����Դϴ�.
public:
	virtual ~CMemMonitorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MemMonitorView.cpp�� ����� ����
inline CMemMonitorDoc* CMemMonitorView::GetDocument() const
   { return reinterpret_cast<CMemMonitorDoc*>(m_pDocument); }
#endif

