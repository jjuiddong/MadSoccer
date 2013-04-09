
#include "stdafx.h"
#include "SubServerGroup.h"
#include "RemoteSubServer.h"

using namespace network;

/**
 @brief clientSvrType 이 serverSvrType 서버의 클라이언트로 접속할 때, serverSvrType 의 ip 와 port 정보를 
 v 에 저장해 리턴한다.
 serverSvrType 타입의 서버가 bind 중일 때만 저장한다.
 */
bool CSubServerGroup::GetServerInfo( const std::string &clientSvrType, const std::string &serverSvrType, 
	IUserAccess &userAccess, OUT std::vector<SHostInfo> &v)
{
	BOOST_FOREACH(auto &netId, GetUsers())
	{
		RemoteSubServerPtr pSubServer = dynamic_cast<CRemoteSubServer*>(
			userAccess.GetUser(netId).Get());
		if (!pSubServer)
			continue;
		pSubServer->GetLinkInfo( clientSvrType, v );
	}
	return true;
}


/**
 @brief GetToBindOuterPort
 */
int CSubServerGroup::GetToBindOuterPort(IUserAccess &userAccess)
{
	int port = m_OuterPortBase;
	BOOST_FOREACH(auto &netId, GetUsers())
	{
		RemoteSubServerPtr pSubServer = dynamic_cast<CRemoteSubServer*>(
			userAccess.GetUser(netId).Get());
		if (!pSubServer)
			continue;
		port = pSubServer->GetToBindOuterPort(port);
	}
	return port;
}


/**
 @brief GetToBindInnerPort
 */
int CSubServerGroup::GetToBindInnerPort(IUserAccess &userAccess)
{
	int port = m_InnerPortBase;
	BOOST_FOREACH(auto &netId, GetUsers())
	{
		RemoteSubServerPtr pSubServer = dynamic_cast<CRemoteSubServer*>(
			userAccess.GetUser(netId).Get());
		if (!pSubServer)
			continue;
		port = pSubServer->GetToBindInnerPort(port);
	}
	return port;
}
