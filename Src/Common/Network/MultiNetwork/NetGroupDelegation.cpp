
#include "stdafx.h"
#include "NetGroupDelegation.h"
#include "NetGroupController.h"

using namespace network;
using namespace network::multinetwork;


CNetGroupDelegation::CNetGroupDelegation( const std::string svrType ) :
	m_SvrType(svrType)
{
}

CNetGroupDelegation::~CNetGroupDelegation()
{
}

void	 CNetGroupDelegation::SetConnector(NetGroupControllerPtr ptr) 
{ 
	CNetConnector *p = dynamic_cast<CNetConnector*>(ptr.Get());
	SetNetConnector( p); 
}

NetGroupControllerPtr CNetGroupDelegation::GetConnector() const 
{ 
	return GetNetConnector(); 
}
