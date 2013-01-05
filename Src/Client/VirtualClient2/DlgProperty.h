//------------------------------------------------------------------------
// Name:    DlgProperty.h
// Author:  jjuiddong
// Date:    1/3/2013
// 
// CMFCPropertyGridCtrl �� �����ϴ� ���̾�α״�.
//------------------------------------------------------------------------
#pragma once

#include "Network/PrtCompiler/ProtocolDefine.h"
#include "afxwin.h"

class CDlgProperty : public CDialog
{
	DECLARE_DYNAMIC(CDlgProperty)
public:
	CDlgProperty(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgProperty();
	enum { IDD = IDD_DLG_PROPERTY,
		IDC_BUTTON_SEND=100};

protected:
	typedef struct _SPropItem
	{
		CMFCPropertyGridProperty *prop;

	} SPropItem;
	typedef std::list<SPropItem*> PropList;
	typedef PropList::iterator PropItor;

	PropList				m_PropList;
	network::sProtocol		*m_pProtocol;
	network::sRmi			*m_pRmi;

	CButton					m_BtnSend;
	CMFCPropertyGridCtrl	m_wndPropList;

public:
	void					UpdateProperty(network::sRmi *rmi, network::sProtocol *protocol);

protected:
	void					AddProperty(CMFCPropertyGridProperty *prop);
	void					DeleteAllProperty();
	void					MakeProperty(network::sProtocol *protocol);
	void					MakePropertyItem(network::sArg *arg);
	void					MakePacket( network::CPacket &packet, network::sRmi *rmi, network::sProtocol *protocol );
	void					MakePacketProperty( network::CPacket &packet, PropItor it, network::sArg *arg);
	

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
