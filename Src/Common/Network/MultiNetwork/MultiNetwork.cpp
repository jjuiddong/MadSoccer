
#include "stdafx.h"
#include "MultiNetwork.h"
#include "MultiNetworkUtillity.h"
#include "NetGroupController.h"
#include "NetGroupDelegation.h"
#include "FarmServerConnector.h"


using namespace network;
using namespace network::multinetwork;


CMultiNetwork::CMultiNetwork() :
	m_pFarmSvrConnector(NULL)
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

	if ("farmsvr" == m_Config.svrType)
	{
		CNetGroupController *pMainCtrl = new CNetGroupController(SERVER, m_Config.svrType, "client");
		if (!AddController(pMainCtrl))
			return false;
	}
	else
	{
		// create farmsvr connector, and delegation
		if (!m_Config.parentSvrIp.empty())
		{
			CNetGroupController *pCtrl = new CNetGroupController(CLIENT, m_Config.svrType, "farmsvr");
			if (!AddController(pCtrl))
				return false;

			m_pFarmSvrConnector = new CFarmServerConnector( m_Config.svrType, m_Config );
			if (!ConnectDelegation("farmsvr", m_pFarmSvrConnector))
				return false;
		}
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

	SAFE_DELETE( m_pFarmSvrConnector );

}


/**
 @brief 
 */
bool	CMultiNetwork::ConnectDelegation( const std::string &linkSvrType, NetGroupDelegationPtr ptr)
{
	RETV(!ptr, false);

	auto it = m_Controllers.find( linkSvrType );
	if (m_Controllers.end() == it)
		return false; // not exist

	ptr->SetConnector( it->second );
	return true;
}



/**
 @brief 
 */
bool	CMultiNetwork::Start()
{
	if ("farmsvr" == m_Config.svrType)
	{
		NetGroupControllerPtr ptr  = GetController("client");
		if (!ptr) return false;
		ptr->Start("localhost", m_Config.port);
	}
	else
	{
		if (m_pFarmSvrConnector)
			m_pFarmSvrConnector->Start( m_Config.parentSvrIp, m_Config.parentSvrPort );
	}	

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
bool	CMultiNetwork::RemoveController( const std::string &linkSvrType )
{
	auto it = m_Controllers.find( linkSvrType );
	if (m_Controllers.end() == it)
		return false; // not exist

	m_Controllers.remove( linkSvrType );
	return true;
}


/**
 @brief Get NetGroupController Object
 */
NetGroupControllerPtr CMultiNetwork::GetController( const std::string &linkSvrType )
{
	auto it = m_Controllers.find( linkSvrType );
	if (m_Controllers.end() == it)
		return NULL; // not exist
	return it->second;
}

