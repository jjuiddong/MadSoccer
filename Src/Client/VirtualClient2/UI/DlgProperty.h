//------------------------------------------------------------------------
// Name:    DlgProperty.h
// Author:  jjuiddong
// Date:    1/3/2013
// 
// CMFCPropertyGridCtrl 을 관리하는 다이얼로그다.
//------------------------------------------------------------------------
#pragma once

#include "afxwin.h"
#include "ProtocolPropertyCtrl.h"

class CDlgProperty : public CDialog
{
	DECLARE_DYNAMIC(CDlgProperty)
public:
	CDlgProperty(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgProperty();
	enum { IDD = IDD_DLG_PROPERTY,
		IDC_BUTTON_SEND=100,
		IDC_BUTTON_P2PSEND,
	};

protected:
	typedef struct _SPropItem
	{
		CMFCPropertyGridProperty *prop;

	} SPropItem;
	typedef std::list<SPropItem*> PropList;
	typedef PropList::iterator PropItor;

	PropList									m_PropList;
	network::sProtocol					*m_pProtocol;
	network::sRmi							*m_pRmi;

	CButton						m_BtnSend;
	CButton						m_BtnP2PSend;
	CProtocolPropertyCtrl	m_wndPropList;

public:
	void					UpdateProperty(network::sRmi *rmi, network::sProtocol *protocol,
													const std::string &scope );

protected:
	void					AddProperty(CMFCPropertyGridProperty *prop);
	void					DeleteAllProperty();
	void					MakeProperty(network::sProtocol *protocol, const std::string &scope );
	void					MakePropertyItem(network::sArg *arg, const std::string &scope );
	void					MakePacket( network::CPacket &packet, network::sRmi *rmi, network::sProtocol *protocol );
	void					MakePacketProperty( network::CPacket &packet, PropItor it, network::sArg *arg);
	

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonP2PSend();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
