
// MemMonitorDoc.cpp : CMemMonitorDoc Ŭ������ ����
//

#include "stdafx.h"
#include "MemMonitor.h"

#include "MemMonitorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMemMonitorDoc

IMPLEMENT_DYNCREATE(CMemMonitorDoc, CDocument)

BEGIN_MESSAGE_MAP(CMemMonitorDoc, CDocument)
END_MESSAGE_MAP()


// CMemMonitorDoc ����/�Ҹ�

CMemMonitorDoc::CMemMonitorDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CMemMonitorDoc::~CMemMonitorDoc()
{
}

BOOL CMemMonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CMemMonitorDoc serialization

void CMemMonitorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CMemMonitorDoc ����

#ifdef _DEBUG
void CMemMonitorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMemMonitorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMemMonitorDoc ���
