
#include "stdafx.h"
#include "FarmServer.h"
#include "RemoteSubServer.h"
#include "SubServerGroup.h"
#include "Network/Utility/ServerUserAccess.h"
#include "FarmServerUtility.h"

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
void	CFarmServer::OnConnectMultiPlug()
{
	RegisterProtocol( &m_Protocol );
	AddProtocolListener(this);
	GetMultiPlug()->SetSessionFactory( new CRemoteSubServerFactory() );
	GetMultiPlug()->SetGroupFactory( new CSubServerGroupFactory() );

	m_pServer = GetServer();

	if (!farmsvr::ReadServerGrouprConfig( "servergroup_config.json", *this))
	{ // Error!!
		assert(0);
	}

	NETEVENT_CONNECT( EVT_CLIENT_JOIN, CFarmServer, CFarmServer::OnClientJoin );

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
bool CFarmServer::ReqSubServerLogin(IProtocolDispatcher &dispatcher, netid senderId, const std::string &svrType)
{
	GroupPtr pFromGroup = GetServer()->GetRootGroup().GetChildFromUser( senderId );
	if (!pFromGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "ReqSubServerLogin Error!!, not exist group user id = %d\n\n", senderId );
		m_Protocol.AckSubServerLogin( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP);
		return false;
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
			return false;
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
		return false;
	}
	pSvrGroup->AddViewer( GetServer()->GetRootGroup().GetId() );

	m_Protocol.AckSubServerLogin( senderId, SEND_T, error::ERR_SUCCESS);
	return true;
}


/**
 @brief SendSubServerP2PCLink
 */
bool CFarmServer::SendSubServerP2PCLink(IProtocolDispatcher &dispatcher, netid senderId, const std::vector<std::string> &v)
{
	GroupPtr pGroup = GetServer()->GetRootGroup().GetChildFromUser( senderId );
	if (!pGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "SendSubServerP2PCLink Error!!, not exist group user id = %d\n", senderId );
		m_Protocol.AckSendSubServerP2PCLink( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP);
		return false;
	}

	RemoteSubServerPtr pClient = dynamic_cast<CRemoteSubServer*>(
		GetServer()->GetSession(senderId));
	if (!pClient)
	{
		clog::Error( log::ERROR_PROBLEM, "SendSubServerP2PCLink Error!!, not exist user id = %d\n", senderId );
		m_Protocol.AckSendSubServerP2PCLink( senderId, SEND_T, error::ERR_NOT_FOUND_USER );
		return false;
	}

	pClient->SetP2PCLink( v );
	m_Protocol.AckSendSubServerP2PCLink( senderId, SEND_T, error::ERR_SUCCESS );
	return true;
}


/**
 @brief SendSubServerP2PSLink
 */
bool CFarmServer::SendSubServerP2PSLink(IProtocolDispatcher &dispatcher, netid senderId, const std::vector<std::string> &v)
{
	GroupPtr pGroup = GetServer()->GetRootGroup().GetChildFromUser( senderId );
	if (!pGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "SendSubServerP2PSLink Error!!, not exist group user id = %d\n", senderId );
		m_Protocol.AckSendSubServerP2PSLink( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP);
		return false;
	}	

	RemoteSubServerPtr pClient = dynamic_cast<CRemoteSubServer*>(
		GetServer()->GetSession(senderId));
	if (!pClient)
	{
		clog::Error( log::ERROR_PROBLEM, "SendSubServerP2PSLink Error!!, not exist user id = %d\n", senderId );
		m_Protocol.AckSendSubServerP2PSLink( senderId, SEND_T, error::ERR_NOT_FOUND_USER );
		return false;
	}

	pClient->SetP2PSLink( v );
	m_Protocol.AckSendSubServerP2PSLink( senderId, SEND_T, error::ERR_SUCCESS );
	return true;
}


/**
 @brief SendSubServerInputLink
 */
bool CFarmServer::SendSubServerInputLink(IProtocolDispatcher &dispatcher, netid senderId, const std::vector<std::string> &v)
{
	GroupPtr pGroup = GetServer()->GetRootGroup().GetChildFromUser( senderId );
	if (!pGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "SendSubServerInputLink Error!!, not exist group user id = %d\n", senderId );
		m_Protocol.AckSendSubServerInputLink( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP);
		return false;
	}	

	RemoteSubServerPtr pClient = dynamic_cast<CRemoteSubServer*>(
		GetServer()->GetSession(senderId));
	if (!pClient)
	{
		clog::Error( log::ERROR_PROBLEM, "SendSubServerInputLink Error!!, not exist user id = %d\n", senderId );
		m_Protocol.AckSendSubServerInputLink( senderId, SEND_T, error::ERR_NOT_FOUND_USER );
		return false;
	}

	pClient->SetInputLink( v );
	m_Protocol.AckSendSubServerInputLink( senderId, SEND_T, error::ERR_SUCCESS );
	return true;
}


/**
 @brief SendSubServerOutputLink
 */
bool CFarmServer::SendSubServerOutputLink(IProtocolDispatcher &dispatcher, netid senderId, const std::vector<std::string> &v)
{
	GroupPtr pGroup = GetServer()->GetRootGroup().GetChildFromUser( senderId );
	if (!pGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "SendSubServerOutputLink Error!!, not exist group user id = %d\n", senderId );
		m_Protocol.AckSendSubServerOutputLink(senderId, SEND_T, error::ERR_NOT_FOUND_GROUP);
		return false;
	}	

	RemoteSubServerPtr pClient = dynamic_cast<CRemoteSubServer*>(
		GetServer()->GetSession(senderId));
	if (!pClient)
	{
		clog::Error( log::ERROR_PROBLEM, "SendSubServerOutputLink Error!!, not exist user id = %d\n", senderId );
		m_Protocol.AckSendSubServerOutputLink( senderId, SEND_T, error::ERR_NOT_FOUND_USER );
		return false;
	}

	pClient->SetOutputLink( v );
	m_Protocol.AckSendSubServerOutputLink( senderId, SEND_T, error::ERR_SUCCESS );
	return true;
}


/**
 @brief ReqServerInfoList
 */
bool CFarmServer::ReqServerInfoList(IProtocolDispatcher &dispatcher, netid senderId, const std::string &clientSvrType, const std::string &serverSvrType)
{
	std::vector<network::SHostInfo> hostInfo;
	GroupPtr pGroup = GetServer()->GetRootGroup().GetChildFromUser( senderId );
	if (!pGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "ReqServerInfoList Error!!, not exist group user id = %d\n", senderId );
		m_Protocol.AckServerInfoList( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP, clientSvrType, serverSvrType, hostInfo);
		return false;
	}
	
	SubServerGroupPtr pServerGroup = FindGroup( serverSvrType );
	if (!pServerGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "ReqServerInfoList Error!!, not exist serverSvrType group = %s\n", serverSvrType.c_str() );
		m_Protocol.AckServerInfoList( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP, clientSvrType, serverSvrType, hostInfo);
		return false;
	}

	hostInfo.reserve(10);
	pServerGroup->GetServerInfoCorrespondClient(clientSvrType, serverSvrType, CServerUserAccess(GetServer()), hostInfo);

	if (hostInfo.empty())
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "ReqServerInfoList Error!!, not found ServerInfo svrType = %s\n", clientSvrType.c_str() );
	}

	m_Protocol.AckServerInfoList( senderId, SEND_T, 
		(hostInfo.size() <= 0)? error::ERR_REQSERVERINFO_NOTFOUND_SERVER : error::ERR_SUCCESS, 
		clientSvrType, serverSvrType, hostInfo );
	return true;
}


/**
 @brief ReqToBindOuterPort
 */
bool CFarmServer::ReqToBindOuterPort(IProtocolDispatcher &dispatcher, netid senderId, const std::string &bindSubServerSvrType)
{
	GroupPtr pGroup = GetServer()->GetRootGroup().GetChildFromUser( senderId );
	if (!pGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "ReqToBindOuterPort Error!!, not exist group user id = %d\n", senderId );
		m_Protocol.AckToBindOuterPort( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP, bindSubServerSvrType, 0);
		return false;
	}

	SubServerGroupPtr pSubSvrGroup = dynamic_cast<CSubServerGroup*>(pGroup.Get());
	if (!pSubSvrGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "ReqToBindOuterPort Error!!, not convert group user id = %d\n", senderId );
		m_Protocol.AckToBindOuterPort( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP, bindSubServerSvrType, 0);
		return false;
	}

	const int bindPort = pSubSvrGroup->GetToBindOuterPort( CServerUserAccess(GetServer()) );

	RemoteSubServerPtr pSubServer = dynamic_cast<CRemoteSubServer*>(
		GetServer()->GetSession(senderId));
	if (!pSubServer)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "ReqToBindOuterPort Error!!, not found user user id = %d\n", senderId );
		m_Protocol.AckToBindOuterPort( senderId, SEND_T, error::ERR_NOT_FOUND_USER, bindSubServerSvrType, 0);
		return false;
	}

	pSubServer->SetOuterBindPort( "client", bindPort );
	m_Protocol.AckToBindOuterPort( senderId, SEND_T, error::ERR_SUCCESS, bindSubServerSvrType, bindPort );
	return true;
}


/**
 @brief ReqToBindInnerPort
 */
bool CFarmServer::ReqToBindInnerPort(IProtocolDispatcher &dispatcher, netid senderId, const std::string &bindSubServerSvrType)
{
	GroupPtr pGroup = GetServer()->GetRootGroup().GetChildFromUser( senderId );
	if (!pGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "ReqToBindInnerPort Error!!, not exist group user id = %d\n", senderId );
		m_Protocol.AckToBindOuterPort( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP, bindSubServerSvrType, 0);
		return false;
	}

	SubServerGroupPtr pSubSvrGroup = dynamic_cast<CSubServerGroup*>(pGroup.Get());
	if (!pSubSvrGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "ReqToBindInnerPort Error!!, not convert group user id = %d\n", senderId );
		m_Protocol.AckToBindOuterPort( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP, bindSubServerSvrType, 0);
		return false;
	}

	const int bindPort = pSubSvrGroup->GetToBindInnerPort( CServerUserAccess(GetServer()) );

	RemoteSubServerPtr pSubServer = dynamic_cast<CRemoteSubServer*>(
		GetServer()->GetSession(senderId));
	if (!pSubServer)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "ReqToBindInnerPort Error!!, not found user user id = %d\n", senderId );
		m_Protocol.AckToBindOuterPort( senderId, SEND_T, error::ERR_NOT_FOUND_USER, bindSubServerSvrType, 0);
		return false;
	}

	pSubServer->SetInnerBindPort( bindSubServerSvrType, bindPort );
	m_Protocol.AckToBindInnerPort( senderId, SEND_T, error::ERR_SUCCESS, bindSubServerSvrType, bindPort );
	return true;
}


/**
 @brief ReqSubServerBindComplete
 */
bool CFarmServer::ReqSubServerBindComplete(IProtocolDispatcher &dispatcher, netid senderId, const std::string &bindSubServerSvrType)
{
	GroupPtr pGroup = GetServer()->GetRootGroup().GetChildFromUser( senderId );
	if (!pGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "ReqSubServerBindComplete Error!!, not exist group user id = %d\n", senderId );
		m_Protocol.AckSubServerBindComplete( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP, bindSubServerSvrType );
		return false;
	}

	SubServerGroupPtr pSubSvrGroup = dynamic_cast<CSubServerGroup*>(pGroup.Get());
	if (!pSubSvrGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "ReqSubServerBindComplete Error!!, not convert group user id = %d\n", senderId );
		m_Protocol.AckSubServerBindComplete( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP, bindSubServerSvrType );
		return false;
	}

	RemoteSubServerPtr pClient = dynamic_cast<CRemoteSubServer*>(
		GetServer()->GetSession(senderId));
	if (!pClient)
	{
		clog::Error( log::ERROR_PROBLEM, "AckSubServerBindComplete Error!!, not exist user id = %d\n", senderId );
		m_Protocol.AckSubServerBindComplete( senderId, SEND_T, error::ERR_NOT_FOUND_USER, bindSubServerSvrType );
		return false;
	}

	pClient->SetBindComplete(bindSubServerSvrType);
	m_Protocol.AckSubServerBindComplete( senderId, SEND_T, error::ERR_SUCCESS, bindSubServerSvrType );

	if (bindSubServerSvrType == "client")
		return false;

	// pClient 에 p2p link 가 있거나, input_link 가 있는 서버에게 메세지를 보낸다.
	// 다시 해석하면 pClient에게 p2pS link, output_link 인 서버에게 메세지를 보낸다.
	std::vector<network::SHostInfo> bindInfo;
	bindInfo.reserve(10);
	pClient->GetServerInfoCorrespondClientLink(bindSubServerSvrType, bindInfo);
	if (bindInfo.empty())
	{
		clog::Error( clog::ERROR_PROBLEM, "Not Found Bind Server binSvrType : %s", bindSubServerSvrType.c_str() );
		return false;
	}
	if (bindInfo.size() > 1)
	{
		clog::Error( clog::ERROR_CRITICAL, "Too Many Bind Server Found binSvrType : %s", bindSubServerSvrType.c_str() );
		return false;
	}

	std::vector<std::string> links;
	pSubSvrGroup->GetCorrespondClientInfo( CServerUserAccess(GetServer()), links );
	BOOST_FOREACH(auto &svrType, links)
	{
		SubServerGroupPtr pGroup = FindGroup(svrType);
		if (!pGroup)
			continue;
		m_Protocol.BindSubServer( pGroup->GetId(), SEND_T, pSubSvrGroup->GetSvrType(), 
			bindInfo.front().ip, bindInfo.front().portnum );
	}
	return true;
}


/**
 @brief ReqSubClientConnectComplete
 */
bool CFarmServer::ReqSubClientConnectComplete(IProtocolDispatcher &dispatcher, netid senderId, const std::string &bindSubServerSvrType)
{
	GroupPtr pGroup = GetServer()->GetRootGroup().GetChildFromUser( senderId );
	if (!pGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "AckSubClientConnectComplete Error!!, not exist group user id = %d\n", senderId );
		m_Protocol.AckSubClientConnectComplete( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP, bindSubServerSvrType );
		return false;
	}

	SubServerGroupPtr pSubSvrGroup = dynamic_cast<CSubServerGroup*>(pGroup.Get());
	if (!pSubSvrGroup)
	{// Error!!
		clog::Error( log::ERROR_PROBLEM, "AckSubClientConnectComplete Error!!, not convert group user id = %d\n", senderId );
		m_Protocol.AckSubClientConnectComplete( senderId, SEND_T, error::ERR_NOT_FOUND_GROUP, bindSubServerSvrType );
		return false;
	}

	RemoteSubServerPtr pClient = dynamic_cast<CRemoteSubServer*>(
		GetServer()->GetSession(senderId));
	if (!pClient)
	{
		clog::Error( log::ERROR_PROBLEM, "AckSubClientConnectComplete Error!!, not exist user id = %d\n", senderId );
		m_Protocol.AckSubClientConnectComplete( senderId, SEND_T, error::ERR_NOT_FOUND_USER, bindSubServerSvrType );
		return false;
	}

	pClient->SetConnectComplete(bindSubServerSvrType);
	m_Protocol.AckSubClientConnectComplete( senderId, SEND_T, error::ERR_SUCCESS, bindSubServerSvrType );
	return true;
}

