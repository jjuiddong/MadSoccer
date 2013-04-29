
#include "stdafx.h"
#include "MultiPlugDelegation.h"
#include "MultiPlug.h"

using namespace network;
using namespace network::multinetwork;


CMultiPlugDelegation::CMultiPlugDelegation()
	//m_SvrType(linkSvrType)
{
}

CMultiPlugDelegation::~CMultiPlugDelegation()
{
}

void	 CMultiPlugDelegation::SetConnector(NetGroupControllerPtr ptr) 
{ 
	CPlug *p = dynamic_cast<CPlug*>(ptr.Get());
	SetPlug( p); 
	if (p)
		OnConnectNetGroupController(); /// Call Event Handler
}

NetGroupControllerPtr CMultiPlugDelegation::GetConnector() const 
{ 
	return GetPlug(); 
}


/**
 @brief 
 */
CServerBasic* CMultiPlugDelegation::GetServer()
{
	RETV(!GetConnector(), NULL);
	return GetConnector()->GetServer();
}


/**
 @brief 
 */
const CoreClients_V& CMultiPlugDelegation::GetClients()
{
	static CoreClients_V v;
	RETV(!GetConnector(), v);
	return GetConnector()->GetClients();
}


/**
 @brief 
 */
CoreClientPtr CMultiPlugDelegation::GetClient(netid netId)
{
	RETV(!GetConnector(), NULL);	
	return GetConnector()->GetClient(netId);
}


/**
 @brief 
 */
CoreClientPtr CMultiPlugDelegation::GetClientFromServerNetId(netid serverNetId)
{
	RETV(!GetConnector(), NULL);	
	return GetConnector()->GetClientFromServerNetId(serverNetId);
}

