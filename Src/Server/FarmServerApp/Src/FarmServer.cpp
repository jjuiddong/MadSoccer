
#include "stdafx.h"
#include "FarmServer.h"
#include "RemoteSubServer.h"
#include "SubServerGroup.h"
#include "Network/Utillity/ServerUserAccess.h"
#include "FarmServerUtillity.h"

using namespace network;

CFarmServer::CFarmServer() 
{
}

CFarmServer::~CFarmServer()
{
}


/**
 @brief NetGroupController 와 연결되면 호출된다.
 */
void	CFarmServer::OnConnectNetGroupController()
{
	RegisterProtocol( &m_Protocol );
	AddProtocolListener(this);
	GetConnector()->SetRemoteClientFactory( new CRemoteSubServerFactory() );
	GetConnector()->SetGroupFactory( new CSubServerGroupFactory() );

	m_pServer = GetServer();

	if (!farmsvr::ReadServerGrouprConfig( "servergroup_config.json", *this))
	{ // Error!!
		assert(0);
	}
}


/**
 @brief 
 */
SubServerGroupPtr CFarmServer::FindGroup(const std::string &svrType)
{
	BOOST_FOREACH(auto &grp, GetServer()->GetRootGroup().GetChildren())
	{
		CSubServerGroup *pGroup = dynamic_cast<CSubServerGroup*>(grp);
		if (!pGroup)
			continue;
		if (pGroup->GetSvrType() == svrType)
			return pGroup;
	}
	return NULL;
}


/**
@brief ReqSubServerLogin
*/
void CFarmServer::ReqSubServerLogin(netid senderId, const std::string &svrType)
{
	GroupPtr pFromGroup = GetServer()->GetRootGroup().GetChildFromUser( senderId );
	if (!pFromGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "ReqSubServerLogin Error!!, not exist group user id = %d\n\n", senderId );
		m_Protocol.AckSubServerLogin( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP);
		return;
	}

	SubServerGroupPtr pSvrGroup = FindGroup(svrType);
	if (!pSvrGroup)
	{
		//SubServerGroupPtr pNewGroup = dynamic_cast<CSubServerGroup*>(
		//	GetServer()->GetRootGroup().AddChild( GetServer()->GetGroupFactory()) );
		//if (!pNewGroup)
		{// Error!!
			clog::Error( log::ERROR_PROBLEM, "ReqSubServerLogin Error!!, not exist group %s \n\n", svrType.c_str() );
			m_Protocol.AckSubServerLogin( senderId, SEND_T, error::ERR_NO_CREATE_GROUP);
			return;
		}
		//pSvrGroup = pNewGroup;
	}

	// Waiting Group -> New Group
	pFromGroup->RemoveUser( pFromGroup->GetId(), senderId );
	if (!pSvrGroup->AddUser(pSvrGroup->GetId(), senderId))
	{ // Error!!
		pFromGroup->AddUser(pFromGroup->GetId(), senderId); // 복구

		clog::Error( log::ERROR_PROBLEM, "ReqSubServerLogin Error!!, not join group\n" );
		m_Protocol.AckSubServerLogin( senderId, SEND_T, error::ERR_NOT_JOIN_GROUP);
		return;
	}
	pSvrGroup->AddViewer( GetServer()->GetRootGroup().GetId() );

	m_Protocol.AckSubServerLogin( senderId, SEND_T, error::ERR_SUCCESS);
}


/**
 @brief SendSubServerP2PLink
 */
void CFarmServer::SendSubServerP2PLink(netid senderId, const std::vector<std::string> &v)
{
	GroupPtr pGroup = GetServer()->GetRootGroup().GetChildFromUser( senderId );
	if (!pGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "SendSubServerP2PLink Error!!, not exist group user id = %d\n", senderId );
		m_Protocol.AckSendSubServerP2PLink( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP);
		return;
	}	

	RemoteSubServerPtr pClient = dynamic_cast<CRemoteSubServer*>(
		GetServer()->GetRemoteClient(senderId));
	if (!pClient)
	{
		clog::Error( log::ERROR_PROBLEM, "SendSubServerP2PLink Error!!, not exist user id = %d\n", senderId );
		m_Protocol.AckSendSubServerP2PLink( senderId, SEND_T, error::ERR_NOT_FOUND_USER );
		return;
	}

	pClient->SetP2PLink( v );
	m_Protocol.AckSendSubServerP2PLink( senderId, SEND_T, error::ERR_SUCCESS );
}


/**
 @brief SendSubServerInputLink
 */
void CFarmServer::SendSubServerInputLink(netid senderId, const std::vector<std::string> &v)
{
	GroupPtr pGroup = GetServer()->GetRootGroup().GetChildFromUser( senderId );
	if (!pGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "SendSubServerInputLink Error!!, not exist group user id = %d\n", senderId );
		m_Protocol.AckSendSubServerInputLink( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP);
		return;
	}	

	RemoteSubServerPtr pClient = dynamic_cast<CRemoteSubServer*>(
		GetServer()->GetRemoteClient(senderId));
	if (!pClient)
	{
		clog::Error( log::ERROR_PROBLEM, "SendSubServerInputLink Error!!, not exist user id = %d\n", senderId );
		m_Protocol.AckSendSubServerInputLink( senderId, SEND_T, error::ERR_NOT_FOUND_USER );
		return;
	}

	pClient->SetInputLink( v );
	m_Protocol.AckSendSubServerInputLink( senderId, SEND_T, error::ERR_SUCCESS );
}


/**
 @brief SendSubServerOutputLink
 */
void CFarmServer::SendSubServerOutputLink(netid senderId, const std::vector<std::string> &v)
{
	GroupPtr pGroup = GetServer()->GetRootGroup().GetChildFromUser( senderId );
	if (!pGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "SendSubServerOutputLink Error!!, not exist group user id = %d\n", senderId );
		m_Protocol.AckSendSubServerOutputLink(senderId, SEND_T, error::ERR_NOT_FOUND_GROUP);
		return;
	}	

	RemoteSubServerPtr pClient = dynamic_cast<CRemoteSubServer*>(
		GetServer()->GetRemoteClient(senderId));
	if (!pClient)
	{
		clog::Error( log::ERROR_PROBLEM, "SendSubServerOutputLink Error!!, not exist user id = %d\n", senderId );
		m_Protocol.AckSendSubServerOutputLink( senderId, SEND_T, error::ERR_NOT_FOUND_USER );
		return;
	}

	pClient->SetOutputLink( v );
	m_Protocol.AckSendSubServerOutputLink( senderId, SEND_T, error::ERR_SUCCESS );
}


/**
 @brief ReqServerInfoList
 */
void CFarmServer::ReqServerInfoList(netid senderId, const std::string &clientSvrType, const std::string &serverSvrType)
{
	std::vector<network::SHostInfo> hostInfo;
	GroupPtr pGroup = GetServer()->GetRootGroup().GetChildFromUser( senderId );
	if (!pGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "ReqServerInfoList Error!!, not exist group user id = %d\n", senderId );
		m_Protocol.AckServerInfoList( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP, clientSvrType, serverSvrType, hostInfo);
		return;
	}
	
	SubServerGroupPtr pServerGroup = FindGroup( serverSvrType );
	if (!pServerGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "ReqServerInfoList Error!!, not exist serverSvrType group = %s\n", serverSvrType.c_str() );
		m_Protocol.AckServerInfoList( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP, clientSvrType, serverSvrType, hostInfo);
		return;
	}

	hostInfo.reserve(10);
	pServerGroup->GetServerInfo(clientSvrType, serverSvrType, CServerUserAccess(GetServer()), hostInfo);
	m_Protocol.AckServerInfoList( senderId, SEND_T, error::ERR_SUCCESS, clientSvrType, serverSvrType, hostInfo );
}


/**
 @brief ReqToBindOuterPort
 */
void CFarmServer::ReqToBindOuterPort(netid senderId, const std::string &bindSubServerSvrType)
{
	GroupPtr pGroup = GetServer()->GetRootGroup().GetChildFromUser( senderId );
	if (!pGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "ReqToBindOuterPort Error!!, not exist group user id = %d\n", senderId );
		m_Protocol.AckToBindOuterPort( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP, bindSubServerSvrType, 0);
		return;
	}

	SubServerGroupPtr pSubSvrGroup = dynamic_cast<CSubServerGroup*>(pGroup.Get());
	if (!pSubSvrGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "ReqToBindOuterPort Error!!, not convert group user id = %d\n", senderId );
		m_Protocol.AckToBindOuterPort( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP, bindSubServerSvrType, 0);
		return;
	}

	const int bindPort = pSubSvrGroup->GetToBindOuterPort( CServerUserAccess(GetServer()) );

	RemoteSubServerPtr pSubServer = dynamic_cast<CRemoteSubServer*>(
		GetServer()->GetRemoteClient(senderId));
	if (!pSubServer)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "ReqToBindOuterPort Error!!, not found user user id = %d\n", senderId );
		m_Protocol.AckToBindOuterPort( senderId, SEND_T, error::ERR_NOT_FOUND_USER, bindSubServerSvrType, 0);
		return;
	}

	pSubServer->SetOuterBindPort( "client", bindPort );
	m_Protocol.AckToBindOuterPort( senderId, SEND_T, error::ERR_SUCCESS, bindSubServerSvrType, bindPort );
}


/**
 @brief ReqToBindInnerPort
 */
void CFarmServer::ReqToBindInnerPort(netid senderId, const std::string &bindSubServerSvrType)
{
	GroupPtr pGroup = GetServer()->GetRootGroup().GetChildFromUser( senderId );
	if (!pGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "ReqToBindInnerPort Error!!, not exist group user id = %d\n", senderId );
		m_Protocol.AckToBindOuterPort( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP, bindSubServerSvrType, 0);
		return;
	}

	SubServerGroupPtr pSubSvrGroup = dynamic_cast<CSubServerGroup*>(pGroup.Get());
	if (!pSubSvrGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "ReqToBindInnerPort Error!!, not convert group user id = %d\n", senderId );
		m_Protocol.AckToBindOuterPort( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP, bindSubServerSvrType, 0);
		return;
	}

	const int bindPort = pSubSvrGroup->GetToBindInnerPort( CServerUserAccess(GetServer()) );

	RemoteSubServerPtr pSubServer = dynamic_cast<CRemoteSubServer*>(
		GetServer()->GetRemoteClient(senderId));
	if (!pSubServer)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "ReqToBindInnerPort Error!!, not found user user id = %d\n", senderId );
		m_Protocol.AckToBindOuterPort( senderId, SEND_T, error::ERR_NOT_FOUND_USER, bindSubServerSvrType, 0);
		return;
	}

	pSubServer->SetInnerBindPort( bindSubServerSvrType, bindPort );
	m_Protocol.AckToBindInnerPort( senderId, SEND_T, error::ERR_SUCCESS, bindSubServerSvrType, bindPort );
}


/**
 @brief 
 */
void CFarmServer::ReqSubServerBindComplete(netid senderId, const std::string &subServerSvrType)
{

}


/**
 @brief 
 */
void CFarmServer::ReqSubClientConnectComplete(netid senderId, const std::string &subClientSvrType)
{

}

