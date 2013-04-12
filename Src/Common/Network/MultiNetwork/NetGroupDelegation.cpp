
#include "stdafx.h"
#include "NetGroupDelegation.h"
#include "NetGroupController.h"

using namespace network;
using namespace network::multinetwork;


CNetGroupDelegation::CNetGroupDelegation()
	//m_SvrType(svrType)
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
CCoreClient* CNetGroupDelegation::GetClient()
{
	RETV(!GetConnector(), NULL);
	return GetConnector()->GetClient();
}
