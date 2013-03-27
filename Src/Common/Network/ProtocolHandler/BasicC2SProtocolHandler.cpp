
#include "stdafx.h"
#include "BasicC2SProtocolHandler.h"
#include "../Utillity/ServerUserAccess.h"
#include "../Algorithm/GroupTraverse.h"

#include "NetProtocol/Src/basic_ProtocolListener.cpp"
#include "NetProtocol/Src/basic_Protocol.cpp"

using namespace network;
using namespace network::error;

CBasicC2SProtocolHandler::CBasicC2SProtocolHandler( CServer &svr ) :
	m_Server(svr)
{
	m_BasicProtocol.SetNetConnector(&svr);
}

CBasicC2SProtocolHandler::~CBasicC2SProtocolHandler()
{
}


//------------------------------------------------------------------------
// groupid : -1 이라면 root 그룹의 자식을 보낸다.
//------------------------------------------------------------------------
void CBasicC2SProtocolHandler::ReqGroupList(netid senderId, const netid &groupid)
{
	GroupPtr pGroup = NULL;
	if (ROOT_GROUP_NETID == groupid)
	{
		pGroup = &m_Server.m_RootGroup;
	}
	else
	{
		pGroup = m_Server.m_RootGroup.GetChildandThis(groupid);
	}

	GroupVector gv;
	if (pGroup)
	{
		const Groups::VectorType &children = pGroup->GetChildren();
		gv.reserve(children.size());
		for (u_int i=0; i < children.size(); ++i)
			gv.push_back( *children[i] );
	}
	m_BasicProtocol.AckGroupList(senderId, SEND_TARGET, 
		(pGroup)? ERR_SUCCESS : ERR_GROUPLIST_NOT_FOUND_GROUP, gv);
}


//------------------------------------------------------------------------
// Request Joint the Group of groupid
//------------------------------------------------------------------------
void CBasicC2SProtocolHandler::ReqGroupJoin(netid senderId, const netid &groupid)
{
	GroupPtr pTo = (groupid == INVALID_NETID)? &m_Server.m_RootGroup : m_Server.m_RootGroup.GetChildandThis(groupid);
	GroupPtr pFrom = m_Server.m_RootGroup.GetChildFromUser( senderId );
	if (pTo && pFrom)
	{
		if (pTo->GetId() == pFrom->GetId())
		{// Error!!
			m_BasicProtocol.AckGroupJoin( senderId, SEND_TARGET, ERR_GROUPJOIN_ALREADY_SAME_GROUP, 
				senderId, groupid );
			return;
		}
		if (!pTo->IsTerminal())
		{// Error!!
			m_BasicProtocol.AckGroupJoin( senderId, SEND_TARGET, ERR_GROUPJOIN_NOT_TERMINAL, 
				senderId, groupid );
		}

		pFrom->RemoveUser(pFrom->GetId(), senderId);
		pTo->AddUser(pTo->GetId(), senderId);
		m_BasicProtocol.AckGroupJoin( pTo->GetId(), SEND_T_V, ERR_SUCCESS , senderId, groupid );
		m_BasicProtocol.AckGroupJoin( pFrom->GetId(), SEND_T_V, ERR_SUCCESS , senderId, groupid );
	}
	else
	{ // Error!!
		m_BasicProtocol.AckGroupJoin( senderId, SEND_TARGET, ERR_NOT_FOUND_GROUP, senderId, groupid );
	}
}


//------------------------------------------------------------------------
// Request Create Group 
// 이미 group에 유저가 있으면 group이 자식으로 group을 생성할 수 없다.
// 만약 이렇게 하려면, group에 소속된 멤버들을 새 그룹에 소속시키고, 
// 현재 group의 자식으로 추가해야 한다. (단말 노드에만 유저가 소속될 수 있다.)
//------------------------------------------------------------------------
void CBasicC2SProtocolHandler::ReqGroupCreate(netid senderId, const netid &parentGroupId, const std::string &groupName)
{
	GroupPtr pParentGroup, pFrom, pNewGroup;
	if (!CreateBlankGroup(senderId, parentGroupId, groupName, pParentGroup, pFrom, pNewGroup))
		return;

	pFrom->RemoveUser(pFrom->GetId(), senderId);
	pNewGroup->AddUser(pNewGroup->GetId(), senderId);
	pNewGroup->AddViewer( pParentGroup->GetId() );

	const netid groupId = pNewGroup->GetId();
	m_BasicProtocol.AckGroupCreate( pNewGroup->GetId(), SEND_T_V, ERR_SUCCESS, 
		senderId, groupId, parentGroupId, groupName);
	m_BasicProtocol.AckGroupJoin( pNewGroup->GetId(), SEND_T_V, ERR_SUCCESS,
		senderId, groupId);
}


/**
 @brief Create Blank Group
 */
void CBasicC2SProtocolHandler::ReqGroupCreateBlank(
	netid senderId, const netid &parentGroupId, const std::string &groupName)
{
	GroupPtr pParentGroup, pFrom, pNewGroup;
	if (!CreateBlankGroup(senderId, parentGroupId, groupName, pParentGroup, pFrom, pNewGroup))
		return;

	pNewGroup->AddViewer( pParentGroup->GetId() );
	m_BasicProtocol.AckGroupCreateBlank( pNewGroup->GetId(), SEND_T_V, ERR_SUCCESS, 
		senderId, pNewGroup->GetId(), parentGroupId, groupName);
}


/**
 @brief Create Blank Group
 
 이미 group에 유저가 있으면 group이 자식으로 group을 생성할 수 없다.
 만약 이렇게 하려면, group에 소속된 멤버들을 새 그룹에 소속시키고, 
 현재 group의 자식으로 추가해야 한다. 
 
 단말 노드에는 그룹을 추가할 수 없다.
 단말 노드에만 유저가 소속될 수 있다. 
 */
bool	CBasicC2SProtocolHandler::CreateBlankGroup( 
	netid senderId, const netid &parentGroupId, const std::string &groupName, 
	OUT GroupPtr &pParent, OUT GroupPtr &pFrom, OUT GroupPtr &pNew )
{
	GroupPtr pParentGroup = 
		(parentGroupId == INVALID_NETID)? &m_Server.m_RootGroup : m_Server.m_RootGroup.GetChildandThis(parentGroupId);
	if (!pParentGroup)
	{ // Error!!
		m_BasicProtocol.AckGroupCreate( senderId, SEND_TARGET, 
			ERR_GROUPCREATE_NOT_FOUND_PARENT_GROUP, senderId, 0, parentGroupId, groupName );
		return false;
	}

	if (pParentGroup->IsTerminal())
	{// Error!!
		m_BasicProtocol.AckGroupCreate( senderId, SEND_TARGET, 
			ERR_GROUPCREATE_PARENT_TERMINALNODE, senderId, 0, parentGroupId, groupName );
		return false;
	}

	GroupPtr pFromGroup = m_Server.m_RootGroup.GetChildFromUser( senderId );
	if (!pFromGroup)
	{ // Error!!
		m_BasicProtocol.AckGroupCreate( senderId, SEND_TARGET, ERR_NOT_FOUND_USER, 
			senderId, 0, parentGroupId, groupName );
		return false;
	}

	CGroup *pNewGroup = new CGroup(pParentGroup, groupName);
	const bool result = pParentGroup->AddChild( pNewGroup );
	if (!result) 
	{ // Error!!
		SAFE_DELETE(pNewGroup);
		m_BasicProtocol.AckGroupCreate( senderId, SEND_TARGET, ERR_GROUPCREATE_NOR_MORE_CREATE_GROUP, 
			senderId, 0, parentGroupId, groupName );
		return false;
	}

	pParent = pParentGroup;
	pFrom = pFromGroup;
	pNew = pNewGroup;
	return true;
}


//------------------------------------------------------------------------
// Request peer to peer connection
//------------------------------------------------------------------------
void CBasicC2SProtocolHandler::ReqP2PConnect(netid senderId)
{
	// check p2p connection
	// if networking this group on p2p
	// -> then connect to p2p host with this senderId client
	// else
	// -> select p2p host in group client, and network p2p each other

	CRemoteClient* pClient = m_Server.GetRemoteClient(senderId);
	if (!pClient)
	{
		clog::Error( clog::ERROR_PROBLEM, "not found remoteclient netid: %d", senderId );
		return;
	}

	GroupPtr pGroup = m_Server.m_RootGroup.GetChildFromUser(senderId);
	if (!pGroup)
	{
		clog::Error( clog::ERROR_PROBLEM, "not found group from user id: %d", senderId );
		return;
	}

	// search up and down p2p connection
	// if already connect p2p, then this group is p2p connection group
	const netid p2pHostClient = group::GetP2PHostClient(pGroup, CServerUserAccess(&m_Server));
	if (INVALID_NETID != p2pHostClient)
	{
		if (p2pHostClient == senderId)
		{
			// error!!
			m_BasicProtocol.AckP2PConnect( senderId, SEND_TARGET, 
				error::ERR_P2PCONNECTION_ALREADY_CONNECTED, network::P2P_CLIENT, "", 0);
		}
		else
		{
			CRemoteClient *pHostClient = m_Server.GetRemoteClient(p2pHostClient);
			if (!pHostClient)
			{
				// error!!
				// maybe~ never happen this accident
				m_BasicProtocol.AckP2PConnect( senderId, SEND_TARGET, 
					error::ERR_P2PCONNECTION_HOSTCLIENT_DISAPPEAR, network::P2P_CLIENT, "", 0);
				// waitting next command. maybe good work
			}
			else
			{
				// connect p2p client
				m_BasicProtocol.AckP2PConnect( senderId, SEND_TARGET, 
					error::ERR_SUCCESS, network::P2P_CLIENT, pHostClient->GetIp(), 2400);
			}
		}
	}
	else
	{
		const netid newHostClient = group::SelectP2PHostClient(pGroup);
		if (INVALID_NETID == newHostClient)
		{
			// error!!
			m_BasicProtocol.AckP2PConnect( senderId, SEND_TARGET, 
				error::ERR_P2PCONNECTION_NO_MEMBER_IN_GROUP, network::P2P_CLIENT, " ", 0 );
			return;
		}

		// p2p host 
		// build p2p network
		pClient->SetP2PState(P2P_HOST);
		pGroup->SetNetState(CGroup::NET_STATE_P2P);
		m_BasicProtocol.AckP2PConnect( senderId, SEND_TARGET, error::ERR_SUCCESS, network::P2P_HOST, "", 2400 );
	}
}

