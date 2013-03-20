
#include "stdafx.h"
#include "BasicC2SProtocolHandler.h"
#include "../Utillity/ServerUserAccess.h"
#include "../Algorithm/GroupTraverse.h"

#include "NetCommon/Src/basic_ProtocolListener.cpp"
#include "NetCommon/Src/basic_Protocol.cpp"

using namespace network;

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
		pGroup = m_Server.m_RootGroup.GetChild(groupid);
	}

	GroupVector gv;
	if (pGroup)
	{
		const Groups::VectorType &children = pGroup->GetChildren();
		gv.reserve(children.size());
		for (u_int i=0; i < children.size(); ++i)
			gv.push_back( *children[i] );
	}
	m_BasicProtocol.AckGroupList(senderId, SEND_TARGET, (pGroup)? 0 : 1, gv);
}


//------------------------------------------------------------------------
// Request Joint the Group of groupid
//------------------------------------------------------------------------
void CBasicC2SProtocolHandler::ReqGroupJoin(netid senderId, const netid &groupid)
{
	GroupPtr pTo = m_Server.m_RootGroup.GetChild(groupid);
	GroupPtr pFrom = m_Server.m_RootGroup.GetChildFromUser( senderId );
	if (pTo && pFrom)
	{
		pFrom->RemoveUser(pFrom->GetId(), senderId);
		pTo->AddUser(pTo->GetId(), senderId);
		m_BasicProtocol.AckGroupJoin( pTo->GetId(), SEND_T_V, 0 );
	}
	else
	{
		m_BasicProtocol.AckGroupJoin( senderId, SEND_TARGET, 1 );
	}

}


//------------------------------------------------------------------------
// Request Create Group 
//------------------------------------------------------------------------
void CBasicC2SProtocolHandler::ReqGroupCreate(netid senderId, const netid &parentGroupId, const std::string &groupName)
{
	CGroup *pNewGroup = new CGroup(&m_Server.m_RootGroup, groupName);
	const bool result = m_Server.m_RootGroup.AddChild( pNewGroup );
	if (!result) 
	{
		SAFE_DELETE(pNewGroup);
		m_BasicProtocol.AckGroupCreate( senderId, SEND_TARGET, 1, groupName, 0 );
	}

	GroupPtr pFrom = m_Server.m_RootGroup.GetChildFromUser( senderId );
	if (pFrom)
	{
		pFrom->RemoveUser(pFrom->GetId(), senderId);
		pNewGroup->AddUser(pNewGroup->GetId(), senderId);
		pNewGroup->AddViewer( m_Server.m_RootGroup.GetId() );

		const netid groupId = pNewGroup->GetId();
		m_BasicProtocol.AckGroupCreate( pNewGroup->GetId(), SEND_T_V, 0, groupName, groupId );
		m_BasicProtocol.AckGroupJoin( pNewGroup->GetId(), SEND_T_V, 0 );
	}
	else
	{
		m_Server.m_RootGroup.RemoveChild(pNewGroup->GetId());
		m_BasicProtocol.AckGroupCreate( senderId, SEND_TARGET, 1, groupName, 0 );
	}
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
		LogNPrint( "not found remoteclient netid: %d", senderId );
		return;
	}

	GroupPtr pGroup = m_Server.m_RootGroup.GetChildFromUser(senderId);
	if (!pGroup)
	{
		LogNPrint( "not found group from user id: %d", senderId );
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

