
#include "stdafx.h"
#include "MultiNetwork.h"
#include "MultiNetworkUtillity.h"
#include "NetGroupController.h"
#include "NetGroupDelegation.h"
#include "FarmServerConnector.h"


using namespace network;
using namespace network::multinetwork;


CMultiNetwork::CMultiNetwork()
{
	m_Config.clear();

}

CMultiNetwork::~CMultiNetwork()
{
	Cleanup();
}


/**
 @brief 
 */
bool	CMultiNetwork::Init( const std::string &configFileName )
{
	if (!ReadServerConfigFile(configFileName, m_Config))
		return false;

	// create farmsvr connector, and delegation
	if (!m_Config.parentSvrIp.empty())
	{
		CNetGroupController *pCtrl = new CNetGroupController(CLIENT, 
			m_Config.svrType, "farmsvr");
		if (!AddController(pCtrl))
			return false;

		CFarmServerConnector *pDelegation = new CFarmServerConnector( m_Config.svrType );
		if (!ConnectDelegation("farmsvr", pDelegation))
			return false;

		pDelegation->Start( m_Config.parentSvrIp, m_Config.parentSvrPort, m_Config );
	}

	return true;
}


/**
 @brief 
 */
void	CMultiNetwork::Cleanup()
{
	BOOST_FOREACH(auto &ctrl, m_Controllers.m_Seq)
	{
		SAFE_DELETE(ctrl);
	}
	m_Controllers.clear();

}


/**
 @brief 
 */
bool	CMultiNetwork::ConnectDelegation( const std::string &connectSvrType, NetGroupDelegationPtr ptr)
{
	RETV(!ptr, false);

	auto it = m_Controllers.find( connectSvrType );
	if (m_Controllers.end() != it)
		return false; // already exist

	ptr->SetConnector( it->second );
	return true;
}



/**
 @brief 
 */
bool	CMultiNetwork::Start()
{

	return true;
}


/**
 @brief 
 */
bool	CMultiNetwork::Stop()
{

	return true;
}


/**
 @brief 
 */
bool	CMultiNetwork::Proc()
{

	return true;
}


/**
 @brief Add NetGroupController object
 */
bool	CMultiNetwork::AddController( CNetGroupController *ptr )
{
	RETV(!ptr, false);

	auto it = m_Controllers.find( ptr->GetConnectSvrType() );
	if (m_Controllers.end() != it)
		return false; // already exist

	m_Controllers.insert( Controllers::value_type(ptr->GetConnectSvrType(), ptr) );
	return true;
}


/**
 @brief Remove NetGroupController Object
 */
bool	CMultiNetwork::RemoveController( const std::string &svrType )
{
	auto it = m_Controllers.find( svrType );
	if (m_Controllers.end() == it)
		return false; // not exist

	m_Controllers.remove( svrType );
	return true;
}


/**
 @brief Get NetGroupController Object
 */
NetGroupControllerPtr CMultiNetwork::GetController( const std::string &svrType )
{
	auto it = m_Controllers.find( svrType );
	if (m_Controllers.end() == it)
		return NULL; // not exist
	return it->second;
}

