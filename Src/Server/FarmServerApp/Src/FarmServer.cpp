
#include "stdafx.h"
#include "FarmServer.h"
#include "RemoteSubServer.h"
#include "SubServerGroup.h"

using namespace network;

CFarmServer::CFarmServer() : 
	network::multinetwork::CNetGroupDelegation("farmsvr")
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
	GetConnector()->SetRemoteClientFactory( new CRemoteSubServerFactory() );
	GetConnector()->SetGroupFactory( new CSubServerGroupFactory() );
}


/**
 @brief 
 */
void CFarmServer::ReqSubServerLogin(netid senderId, const std::string &svrType)
{
	//GroupPtr *pGroup = GetServer()->GetRootGroup().GetChildFromUser( senderId );
	//if (pGroup)
	//{
	//}

}


/**
 @brief 
 */
void CFarmServer::SendSubServerP2PLink(netid senderId, const std::vector<std::string> &v)
{

}


/**
 @brief 
 */
void CFarmServer::SendSubServerInputLink(netid senderId, const std::vector<std::string> &v)
{

}


/**
 @brief 
 */
void CFarmServer::SendSubServerOutputLink(netid senderId, const std::vector<std::string> &v)
{

}


/**
 @brief 
 */
void CFarmServer::ReqServerInfoList(netid senderId, const std::string &clientSvrType, const std::string &serverSvrType)
{

}


/**
 @brief 
 */
void CFarmServer::ReqToBindOuterPort(netid senderId, const std::string &bindSubServerSvrType)
{

}


/**
 @brief 
 */
void CFarmServer::ReqToBindInnerPort(netid senderId, const std::string &bindSubServerSvrType)
{

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

