// ProtocolTree.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "../VirtualClient2.h"
#include "ProtocolTree.h"
#include "Network/PrtCompiler/ProtocolDefine.h"
#include "Network/PrtCompiler/ProtocolParser.h"
#include "DlgProperty.h"
#include "../Control/ConfigParser.h"
#include "DlgLog.h"

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
// 클래스 초기화
// protocol 파일을 읽어서 TreeCtrl 화면에 출력한다.
// Tree 뷰 구조
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
	if (!config::OpenConfigFile())
	{
		GetConsole()->AddString( config::GetErrorMsg() );
	}

	std::string protocolDir = config::GetProtocolDirectory();
	protocolDir += "\\*.prt";
	std::list<std::string> fileList = common::FindFileList( protocolDir );
	BOOST_FOREACH(std::string &str, fileList)
	{
		CProtocolParser *pParser = new CProtocolParser();
		sRmi *rmiList = pParser->Parse( str.c_str() );
		if (rmiList)
		{
			const std::string protocolName = common::GetFileNameExceptExt(str);
			const HTREEITEM hItem = InsertTree(NULL, 
				common::str2wstr(protocolName).c_str(),
					NewItemInfo(NONE,NULL,NULL,str));
			MakeTreeProtocol(hItem, rmiList, protocolName);

			if (config::IsCollapseNode(protocolName))
				GetConsole()->AddString( protocolName + " collapsed" );
			else
				Expand(hItem, TVE_EXPAND);

			m_ParserList.push_back( pParser );
		}
		else
		{
			delete pParser;
		}
	}

	HTREEITEM hRoot = GetRootItem();
	SelectItem(hRoot);

	return true;
}


//------------------------------------------------------------------------
// Rmi 프로토콜 이름 추가
//------------------------------------------------------------------------
void CProtocolTree::MakeTreeProtocol(HTREEITEM hParentItem, sRmi *rmi,
									 const std::string &scope  )
{
	if (!rmi) return;
	HTREEITEM hItem = InsertTree(hParentItem, common::str2wstr(rmi->name).c_str(), 
		NewItemInfo(RMI,rmi,NULL,scope) );

	const std::string curScope = scope + "::" + rmi->name;
	MakeTreePacket(hItem, rmi, rmi->protocol, curScope );

	if (config::IsCollapseNode(curScope))
		GetConsole()->AddString( curScope + " collapsed" );
	else
		Expand(hItem, TVE_EXPAND);

	MakeTreeProtocol(hParentItem, rmi->next, scope );
}


//------------------------------------------------------------------------
// 프로토콜 추가
//------------------------------------------------------------------------
void CProtocolTree::MakeTreePacket(HTREEITEM hParentItem, sRmi *rmi, sProtocol *protocol,
								   const std::string &scope )
{
	if (!protocol) return;

	std::string name = protocol->name;
	name += "(";
	name += MakeArgString(protocol->argList);
	name += ")";

	HTREEITEM hItem = InsertTree(hParentItem, common::str2wstr(name).c_str(), 
		NewItemInfo(PROTOCOL,rmi,protocol, scope + "::" + protocol->name) );
	Expand(hItem, TVE_EXPAND);

	MakeTreePacket(hParentItem, rmi, protocol->next, scope);
}


//------------------------------------------------------------------------
// 프로토콜 인자를 스트링으로 리턴한다.
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
// SItemInfo구조체를 생성해서 리턴한다.
//------------------------------------------------------------------------
CProtocolTree::SItemInfo* CProtocolTree::NewItemInfo(ITEM_TYPE type, sRmi *rmi, sProtocol *protocol,
													 const std::string &scope )
{
	if (!rmi && !protocol)
		return NULL;
	if (type == NONE)
		return NULL;

	// CDlgProperty 생성
	CDlgProperty *pDlg = new CDlgProperty(GetParent());
	pDlg->Create( CDlgProperty::IDD, GetParent());
	pDlg->ShowWindow(SW_HIDE);
	pDlg->UpdateProperty(rmi, protocol, scope );
	//

	SItemInfo*pItemInfo = new SItemInfo(type, rmi, protocol, pDlg);

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
	{ // 없을때만 삽입
		it->second.insert( ItemInfoSubMap::value_type(packetID, pItemInfo) );
	}
	else
	{
		// 중복 ID 라면 실패
		::AfxMessageBox( _T("중복된 PacketID가 있습니다.") );
		assert(0);
	}

	return pItemInfo;
}


//------------------------------------------------------------------------
// 프로토콜 아이디, 패킷 아이디로 프로토콜 형식을 찾아서 리턴한다.
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
// 트리 노드를 선택할 때 호출된다.
//------------------------------------------------------------------------
void CProtocolTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CString str = GetItemText(pNMTreeView->itemNew.hItem); // 디버깅용
	CString strOld = GetItemText(pNMTreeView->itemOld.hItem); // 디버깅용

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
			CRect pcr;
			GetParent()->GetClientRect(pcr);
			pInfo->dlg->MoveWindow(cr.Width(), 0, pcr.Width()-cr.Width()-4, cr.Height());
			pInfo->dlg->ShowWindow(SW_SHOW);
		}
	}

	*pResult = 0;
}


//------------------------------------------------------------------------
// 메모리 제거
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

	CRect pcr;
	GetParent()->GetClientRect(pcr);
	BOOST_FOREACH(ItemInfoMap::value_type &kv, m_ItemInfoList)
	{
		BOOST_FOREACH(ItemInfoSubMap::value_type &kv2, kv.second)
		{
			kv2.second->dlg->MoveWindow(cx,0,pcr.Width()-cx,cy);
		}
	}	
}
