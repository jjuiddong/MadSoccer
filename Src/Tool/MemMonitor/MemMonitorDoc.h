
// MemMonitorDoc.h : CMemMonitorDoc Ŭ������ �������̽�
//


#pragma once


class CMemMonitorDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CMemMonitorDoc();
	DECLARE_DYNCREATE(CMemMonitorDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �����Դϴ�.
public:
	virtual ~CMemMonitorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


