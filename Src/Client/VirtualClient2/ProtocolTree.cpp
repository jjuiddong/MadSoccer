// ProtocolTree.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "VirtualClient2.h"
#include "ProtocolTree.h"
#include "Network/PrtCompiler/ProtocolDefine.h"
#include "Network/PrtCompiler/ProtocolParser.h"
#include "DlgProperty.h"

using namespace network;

// CProtocolTree

IMPLEMENT_DYNAMIC(CProtocolTree, CTreeCtrlBase)

CProtocolTree::CProtocolTree() : CTreeCtrlBase(TREE_ITEM)
{

}

CProtocolTree::~CProtocolTree()
{

}


BEGIN_MESSAGE_MAP(CProtocolTree, CTreeCtrlBase)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CProtocolTree::OnTvnSelchanged)
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


//------------------------------------------------------------------------
// Ŭ���� �ʱ�ȭ
// protocol ������ �о TreeCtrl ȭ�鿡 ����Ѵ�.
// Tree �� ����
// *.prt file name1
//		rmi name 1
//			-> protocols
//			-> ...
// *.prt file name2 
//		rmi name 2
//			-> protocols
//			-> ...
//------------------------------------------------------------------------
bool CProtocolTree::Init()
{
	std::list<std::string> fileList = common::FindFileList( "C:\\Project\\GitHub\\MadSoccer\\Src\\Common\\NetCommon\\*.prt" );
	BOOST_FOREACH(std::string &str, fileList)
	{
		CProtocolParser *pParser = new CProtocolParser();
		sRmi *rmiList = pParser->Parse( str.c_str() );
		if (rmiList)
		{
			HTREEITEM hItem = InsertTree(NULL, 
				common::string2wstring(common::GetFileNameExceptExt(str)).c_str()
				, NewItemInfo(NONE,NULL,NULL));
			MakeTreeRmi(hItem, rmiList);
			Expand(hItem, TVE_EXPAND);

			m_ParserList.push_back( pParser );
		}
		else
		{
			delete pParser;
		}
	}
	return true;
}


//------------------------------------------------------------------------
// Rmi �������� �̸� �߰�
//------------------------------------------------------------------------
void CProtocolTree::MakeTreeRmi(HTREEITEM hParentItem, sRmi *rmi)
{
	if (!rmi) return;
	HTREEITEM hItem = InsertTree(hParentItem, common::string2wstring(rmi->name).c_str(), 
		NewItemInfo(RMI,rmi,NULL) );
	MakeTreeProtocol(hItem, rmi, rmi->protocol);
	Expand(hItem, TVE_EXPAND);
	MakeTreeRmi(hParentItem, rmi->next);
}


//------------------------------------------------------------------------
// �������� �߰�
//------------------------------------------------------------------------
void CProtocolTree::MakeTreeProtocol(HTREEITEM hParentItem, sRmi *rmi, sProtocol *protocol)
{
	if (!protocol) return;

	std::string name = protocol->name;
	name += "(";
	name += MakeArgString(protocol->argList);
	name += ")";

	HTREEITEM hItem = InsertTree(hParentItem, common::string2wstring(name).c_str(), 
		NewItemInfo(PROTOCOL,rmi,protocol));
	Expand(hItem, TVE_EXPAND);

	MakeTreeProtocol(hParentItem, rmi, protocol->next);
}


//------------------------------------------------------------------------
// �������� ���ڸ� ��Ʈ������ �����Ѵ�.
//------------------------------------------------------------------------
std::string CProtocolTree::MakeArgString(sArg *arg)
{
	if (!arg) return " ";
	std::string name = arg->var->type + " " + arg->var->var;
	if (arg->next)
		return name + ", " + MakeArgString(arg->next);
	else
		return name;
}


//------------------------------------------------------------------------
// SItemInfo����ü�� �����ؼ� �����Ѵ�.
//------------------------------------------------------------------------
CProtocolTree::SItemInfo* CProtocolTree::NewItemInfo(ITEM_TYPE type, sRmi *rmi, sProtocol *protocol)
{
	if (!rmi && !protocol)
		return NULL;
	if (type == NONE)
		return NULL;

	// CDlgProperty ����
	CDlgProperty *pDlg = new CDlgProperty(GetParent());
	pDlg->Create( CDlgProperty::IDD, GetParent());
	pDlg->ShowWindow(SW_HIDE);
	pDlg->UpdateProperty(rmi, protocol);
	//

	SItemInfo*p = new SItemInfo(type, rmi, protocol, pDlg);

	const int protocolID = (rmi? rmi->number : 0);
	ItemInfoMapItor it = m_ItemInfoList.find(protocolID);
	if (m_ItemInfoList.end() == it)
	{
		m_ItemInfoList.insert( ItemInfoMap::value_type(protocolID, ItemInfoSubMap()) );	
		it = m_ItemInfoList.find(protocolID);
	}

	const int packetID = network::GetPacketID(rmi, protocol);
	ItemInfoSubMapItor it2 = it->second.find(packetID);
	if (it->second.end() == it2)
	{ // �������� ����
		it->second.insert( ItemInfoSubMap::value_type(packetID, p) );
	}
	else
	{
		// �ߺ� ID ��� ����
		::AfxMessageBox( _T("�ߺ��� PacketID�� �ֽ��ϴ�.") );
		assert(0);
	}

	return p;
}


//------------------------------------------------------------------------
// �������� ���̵�, ��Ŷ ���̵�� �������� ������ ã�Ƽ� �����Ѵ�.
//------------------------------------------------------------------------
sProtocol* CProtocolTree::GetProtocol(const int protocolID, const int packetID)
{
	ItemInfoMapItor it = m_ItemInfoList.find(protocolID);
	if (m_ItemInfoList.end() == it)
		return NULL;

	ItemInfoSubMapItor it2 = it->second.find(packetID);
	if (it->second.end() == it2)
		return NULL;

	return it2->second->protocol;
}


//------------------------------------------------------------------------
// Ʈ�� ��带 ������ �� ȣ��ȴ�.
//------------------------------------------------------------------------
void CProtocolTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CString str = GetItemText(pNMTreeView->itemNew.hItem); // ������
	CString strOld = GetItemText(pNMTreeView->itemOld.hItem); // ������

	{
		SItemInfo *pInfo = (SItemInfo*)pNMTreeView->itemOld.lParam;
		if (pInfo && pInfo->dlg)
			pInfo->dlg->ShowWindow(SW_HIDE);
	}
	{
		SItemInfo *pInfo = (SItemInfo*)pNMTreeView->itemNew.lParam;
		if (pInfo && pInfo->type == PROTOCOL)
		{
			CRect cr;
			GetClientRect(cr);
			pInfo->dlg->MoveWindow(cr.Width()+1, 0, cr.Width()-1, cr.Height());

			pInfo->dlg->ShowWindow(SW_SHOW);
		}
	}

	*pResult = 0;
}


//------------------------------------------------------------------------
// �޸� ����
//------------------------------------------------------------------------
void CProtocolTree::OnDestroy()
{
	CTreeCtrlBase::OnDestroy();

	BOOST_FOREACH(CProtocolParser *parser, m_ParserList)
	{
		SAFE_DELETE(parser);
	}
	m_ParserList.clear();

	BOOST_FOREACH(ItemInfoMap::value_type &kv, m_ItemInfoList)
	{
		BOOST_FOREACH(ItemInfoSubMap::value_type &kv2, kv.second)
		{
			kv2.second->dlg->DestroyWindow();
			SAFE_DELETE(kv2.second->dlg);
			SAFE_DELETE(kv2.second);
		}
		kv.second.clear();
	}
	m_ItemInfoList.clear();

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CProtocolTree::OnSize(UINT nType, int cx, int cy)
{
	CTreeCtrlBase::OnSize(nType, cx, cy);

	BOOST_FOREACH(ItemInfoMap::value_type &kv, m_ItemInfoList)
	{
		BOOST_FOREACH(ItemInfoSubMap::value_type &kv2, kv.second)
		{
			kv2.second->dlg->MoveWindow(cx,0,cx,cy);
		}
	}	
}
