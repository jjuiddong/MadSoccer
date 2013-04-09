
#include "stdafx.h"
#include "SubServerGroup.h"
#include "RemoteSubServer.h"

using namespace network;

/**
 @brief clientSvrType �� serverSvrType ������ Ŭ���̾�Ʈ�� ������ ��, serverSvrType �� ip �� port ������ 
 v �� ������ �����Ѵ�.
 serverSvrType Ÿ���� ������ bind ���� ���� �����Ѵ�.
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
