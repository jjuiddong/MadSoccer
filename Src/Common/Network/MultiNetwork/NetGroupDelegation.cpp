
#include "stdafx.h"
#include "NetGroupDelegation.h"
#include "NetGroupController.h"

using namespace network;
using namespace network::multinetwork;


CNetGroupDelegation::CNetGroupDelegation()
	//m_SvrType(linkSvrType)
{
}

CNetGroupDelegation::~CNetGroupDelegation()
{
}

void	 CNetGroupDelegation::SetConnector(NetGroupControllerPtr ptr) 
{ 
	CNetConnector *p = dynamic_cast<CNetConnector*>(ptr.Get());
	SetNetConnector( p); 
	if (p)
		OnConnectNetGroupController(); /// Call Event Handler
}

NetGroupControllerPtr CNetGroupDelegation::GetConnector() const 
{ 
	return GetNetConnector(); 
}


/**
 @brief 
 */
CServerBasic* CNetGroupDelegation::GetServer()
{
	RETV(!GetConnector(), NULL);
	return GetConnector()->GetServer();
}


/**
 @brief 
 */
const CoreClients_V& CNetGroupDelegation::GetClients()
{
	static CoreClients_V v;
	RETV(!GetConnector(), v);
	return GetConnector()->GetClients();
}


/**
 @brief 
 */
CoreClientPtr CNetGroupDelegation::GetClient(netid netId)
{
	RETV(!GetConnector(), NULL);	
	return GetConnector()->GetClient(netId);
}


/**
 @brief 
 */
CoreClientPtr CNetGroupDelegation::GetClientFromServerNetId(netid serverNetId)
{
	RETV(!GetConnector(), NULL);	
	return GetConnector()->GetClientFromServerNetId(serverNetId);
}

