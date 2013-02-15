// DlgProperty.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "VirtualClient2.h"
#include "DlgProperty.h"
#include "DlgConsole.h"


using namespace network;

int nBorderSize = 1;

// CDlgProperty ��ȭ �����Դϴ�.
IMPLEMENT_DYNAMIC(CDlgProperty, CDialog)

CDlgProperty::CDlgProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProperty::IDD, pParent)
,	m_pProtocol(NULL)
{

}

CDlgProperty::~CDlgProperty()
{
	m_wndPropList.ExternalRelease();

	BOOST_FOREACH(SPropItem *item, m_PropList)
	{
		SAFE_DELETE(item);
	}
	m_PropList.clear();
}

void CDlgProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgProperty, CDialog)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CDlgProperty::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDOK, &CDlgProperty::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgProperty::OnBnClickedCancel)
END_MESSAGE_MAP()

void CDlgProperty::OnBnClickedOk()
{
}
void CDlgProperty::OnBnClickedCancel()
{
}


// CDlgProperty �޽��� ó�����Դϴ�.
int CDlgProperty::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy(0,0,300,500);
	if (!m_wndPropList.Create (WS_VISIBLE | WS_CHILD, rectDummy, this, 1))
	{
		TRACE0("Failed to create Properies Grid \n");
		return -1;      // fail to create
	}

// 	m_wndPropList.EnableHeaderCtrl (FALSE);
 	m_wndPropList.EnableDescriptionArea ();
// 	m_wndPropList.SetVSDotNetLook ();

	m_BtnSend.Create(_T("Send"), WS_VISIBLE | WS_CHILD|BS_PUSHBUTTON, CRect(0,0,60,30), this, IDC_BUTTON_SEND);

	return 0;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CDlgProperty::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndPropList.GetWindowRect (rectTree);
	ScreenToClient (rectTree);

	rectTree.InflateRect (nBorderSize, nBorderSize);
	dc.Draw3dRect (rectTree,	::GetSysColor (COLOR_3DSHADOW), 
		::GetSysColor (COLOR_3DSHADOW));
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CDlgProperty::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_wndPropList.GetSafeHwnd())
	{
		m_wndPropList.SetWindowPos (NULL, nBorderSize, nBorderSize, 
			cx - 2 * nBorderSize, cy - (2 * nBorderSize) - 60 ,
			SWP_NOACTIVATE | SWP_NOZORDER);
	}

	// Send Button �̵�
	const int btnW=60;
	const int btnH=30;
	const int x = cx/2 - btnW/2;
	const int y = cy-55;
	m_BtnSend.MoveWindow(x, y, btnW, btnH);

	InvalidateRect(NULL);
}


//------------------------------------------------------------------------
// �������� ������ �޾Ƽ� ȭ���� ������Ʈ ��Ų��.
// ���������� ���ڸ� PropertyGridCtrl�� �߰��Ѵ�.
//------------------------------------------------------------------------
void CDlgProperty::UpdateProperty(sRmi *rmi, sProtocol *protocol)
{
	DeleteAllProperty();
	m_pProtocol = protocol;
	m_pRmi = rmi;
	MakeProperty(protocol);
}


//------------------------------------------------------------------------
// �߰��� ��� Property�� �����Ѵ�.
//------------------------------------------------------------------------
void CDlgProperty::DeleteAllProperty()
{
	BOOST_FOREACH(SPropItem *item, m_PropList)
	{
		m_wndPropList.DeleteProperty( item->prop );
		SAFE_DELETE(item);
	}
	m_PropList.clear();	
}


//------------------------------------------------------------------------
// Property�߰�
//------------------------------------------------------------------------
void CDlgProperty::AddProperty(CMFCPropertyGridProperty *prop)
{
	SPropItem *p = new SPropItem();
	p->prop = prop;
	m_PropList.push_back(p);
	m_wndPropList.AddProperty(prop);
}


//------------------------------------------------------------------------
// Property ����
//------------------------------------------------------------------------
void CDlgProperty::MakeProperty(sProtocol *protocol)
{
	if (!protocol) return;
	MakePropertyItem(protocol->argList);
}


//------------------------------------------------------------------------
// Property ����
//------------------------------------------------------------------------
void CDlgProperty::MakePropertyItem(sArg *arg)
{
	if (!arg) return;

	CString str;
	str = common::string2wstring(arg->var->type + " " + arg->var->var).c_str();
	CMFCPropertyGridProperty *pProp = 
		new CMFCPropertyGridProperty(str, GetTypeStr2Type(arg->var->type), _T("Description") );


	AddProperty(pProp);
	MakePropertyItem(arg->next);
}


//------------------------------------------------------------------------
// Send ��ư Ŭ��
//------------------------------------------------------------------------
void CDlgProperty::OnBnClickedButtonSend()
{
	if (!m_pProtocol) 
		return;

	CPacket packet;
	MakePacket(packet, m_pRmi, m_pProtocol);

	// Ư�� ��������, ������ �����ϴ� ���������̴�.
	if (m_pProtocol->name == "Connect")
	{
		int p1,p2;
		packet >> p1 >> p2;
		std::string ip;
		int port;
		packet >> ip >> port;
		network::StartClient(ip, port, CVClient::Get());
	}
	else if (m_pProtocol->name == "DisConnect")
	{
		network::StopClient(CVClient::Get());
	}
	else
	{
		std::stringstream ss;
		ss << "Send = ";
		ss << network::Packet2String(packet, m_pProtocol);
		GetConsole()->AddString( ss.str() );

		CVClient::Get()->GetProtocol().send(SERVER_NETID, packet);
	}

}


//------------------------------------------------------------------------
// packet �� �����Ѵ�.
// property ����Ÿ�� �����ͼ� packet�� ���������� �ִ´�.
//------------------------------------------------------------------------
void CDlgProperty::MakePacket( network::CPacket &packet, sRmi *rmi, sProtocol *protocol )
{
	const int protocolID = rmi->number;
	const int packetID = GetPacketID(rmi, protocol);
	packet << protocolID;
	packet << packetID;
	MakePacketProperty(packet, m_PropList.begin(), protocol->argList);
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CDlgProperty::MakePacketProperty( network::CPacket &packet, PropItor it, network::sArg *arg)
{
	if (m_PropList.end() == it)
		return;
	packet << (_variant_t)(*it)->prop->GetValue();
	MakePacketProperty(packet, ++it, arg->next);
}
