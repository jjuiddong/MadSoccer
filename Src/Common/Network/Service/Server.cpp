
#include "stdafx.h"
#include "server.h"
#include "../Algorithm/GroupTraverse.h"

using namespace network;

CServer::CServer(PROCESS_TYPE procType) :
	CServerBasic(procType)
{
	CGroup *pWaitGroup = new CGroup(NULL,"Waiting Group");
	m_WaitGroupId = pWaitGroup->GetId();
	m_RootGroup.AddChild( pWaitGroup );

	AddListener(this);
	m_BasicProtocol.SetNetConnector(this);

}

CServer::~CServer()
{

}


//------------------------------------------------------------------------
// groupid : -1 이라면 root 그룹의 자식을 보낸다.
//------------------------------------------------------------------------
void CServer::ReqGroupList(netid senderId, const netid &groupid)
{
	GroupPtr pGroup = NULL;
	if (ROOT_GROUP_NETID == groupid)
	{
		pGroup = &m_RootGroup;
	}
	else
	{
		pGroup = m_RootGroup.GetChild(groupid);
	}

	GroupVector gv;
	if (pGroup)
	{
		const Groups::VectorType &children = pGroup->GetChildren();
		gv.reserve(children.size());
		for (u_int i=0; i < children.size(); ++i)
			gv.push_back( *children[i] );
	}
	m_BasicProtocol.AckGroupList(senderId, (pGroup)? 0 : 1, gv);
}


//------------------------------------------------------------------------
// Request Joint the Group of groupid
//------------------------------------------------------------------------
void CServer::ReqGroupJoin(netid senderId, const netid &groupid)
{
	GroupPtr pTo = m_RootGroup.GetChild(groupid);
	GroupPtr pFrom = m_RootGroup.GetChildFromUser( senderId );
	if (pTo && pFrom)
	{
		pFrom->RemoveUser(pFrom->GetId(), senderId);
		pTo->AddUser(pTo->GetId(), senderId);
		m_BasicProtocol.AckGroupJoin( senderId, 0 );
	}
	else
	{
		m_BasicProtocol.AckGroupJoin( senderId, 1 );
	}

}


//------------------------------------------------------------------------
// Request Create Group 
//------------------------------------------------------------------------
void CServer::ReqGroupCreate(netid senderId, const netid &parentGroupId, const std::string &groupName)
{
	CGroup *pNewGroup = new CGroup(&m_RootGroup, groupName);
	const bool result = m_RootGroup.AddChild( pNewGroup );
	if (!result) 
	{
		SAFE_DELETE(pNewGroup);
		m_BasicProtocol.AckGroupCreate( senderId, 1, groupName, 0 );
	}
	
	GroupPtr pFrom = m_RootGroup.GetChildFromUser( senderId );
	if (pFrom)
	{
		pFrom->RemoveUser(pFrom->GetId(), senderId);
		pNewGroup->AddUser(pNewGroup->GetId(), senderId);

		const netid groupId = pNewGroup->GetId();
		m_BasicProtocol.AckGroupCreate( senderId, 0, groupName, groupId );
		m_BasicProtocol.AckGroupJoin( senderId, 0 );
	}
	else
	{
		m_RootGroup.RemoveChild(pNewGroup->GetId());
		m_BasicProtocol.AckGroupCreate( senderId, 1, groupName, 0 );
	}
}


//------------------------------------------------------------------------
// Request peer to peer connection
//------------------------------------------------------------------------
void CServer::ReqP2PConnect(netid senderId)
{
	// check p2p connection
	// if networking this group on p2p
	// -> then connect to p2p host with this senderId client
	// else
	// -> select p2p host in group client, and network p2p each other

	GroupPtr pGroup = m_RootGroup.GetChildFromUser(senderId);
	
	// search up and down p2p connection
	// if already connect p2p, then this group is p2p connection group

	const bool IsP2Pconnection = group::IsP2PConnection(pGroup);


	


}