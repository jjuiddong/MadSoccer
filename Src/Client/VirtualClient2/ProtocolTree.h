//------------------------------------------------------------------------
// Name:    ProtocolTree.h
// Author:  jjuiddong
// Date:    1/3/2013
// 
// �������� Ʈ��
//------------------------------------------------------------------------
#pragma once

#include "Lib/TreeCtrlBase.h"
#include "Network/PrtCompiler/ProtocolDefine.h"

namespace network
{
	class CProtocolParser;
}

class CDlgProperty;
class CProtocolTree : public CTreeCtrlBase
{
	DECLARE_DYNAMIC(CProtocolTree)
public:
	CProtocolTree();
	virtual ~CProtocolTree();

protected:
	enum ITEM_TYPE { NONE, RMI, PROTOCOL};
	typedef struct _SItemInfo
	{
		ITEM_TYPE type;		
		network::sRmi *rmi;				// type�� RMI,PROTOCOL �϶� ��ȿ��
		network::sProtocol *protocol;	// type�� PROTOCOL �϶� ��ȿ��
		CDlgProperty *dlg;
		
		_SItemInfo() {}
		_SItemInfo(ITEM_TYPE _type, network::sRmi *_rmi, network::sProtocol *_protocol, CDlgProperty *_dlg) :
			type(_type), rmi(_rmi), protocol(_protocol), dlg(_dlg) {}

	} SItemInfo;

	typedef std::list<network::CProtocolParser*> ParserList; // rmi�޸� ���Ÿ� �������� �̷�� ���ؼ� ���������.
	typedef ParserList::iterator ParserItor;
	typedef std::map<int,SItemInfo*> ItemInfoSubMap;		// key = packetID
	typedef ItemInfoSubMap::iterator ItemInfoSubMapItor;
	typedef std::map<int,ItemInfoSubMap> ItemInfoMap;		// key = ProtocolID
	typedef ItemInfoMap::iterator ItemInfoMapItor;

	ItemInfoMap			m_ItemInfoList;
	ParserList			m_ParserList;	// rmi�޸� ���Ÿ� �������� �̷�� ���ؼ� ���������.

public:
	bool Init();
	network::sProtocol* GetProtocol(const int protocolID, const int packetID);

protected:
	void MakeTreeRmi(HTREEITEM hParentItem, network::sRmi *rmi);
	void MakeTreeProtocol(HTREEITEM hParentItem, network::sRmi *rmi, network::sProtocol *protocol);
	std::string MakeArgString(network::sArg *arg);
	SItemInfo* NewItemInfo(ITEM_TYPE _type, network::sRmi *_rmi, network::sProtocol *_protocol);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
