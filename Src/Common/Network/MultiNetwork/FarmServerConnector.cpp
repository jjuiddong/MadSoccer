
#include "stdafx.h"
#include "FarmServerConnector.h"
#include "NetGroupController.h"
#include "MultiNetwork.h"
#include "Network/Controller/CoreClient.h"

#include "NetProtocol/Src/farm_Protocol.cpp"
#include "NetProtocol/Src/farm_ProtocolListener.cpp"

using namespace network;
using namespace network::multinetwork;


CFarmServerConnector::CFarmServerConnector(const std::string &svrType, const SSvrConfigData &config) :
	m_IsDetectedSendConfig(false)
{
	m_Config = config;
	CreateLink();
}


/**
@brief OnConnectNetGroupController
*/
void	CFarmServerConnector::OnConnectNetGroupController()
{
	EVENT_CONNECT(EVT_CONNECT, CFarmServerConnector, CFarmServerConnector::OnConnect );
	AddProtocolListener(this);
	RegisterProtocol( &m_Protocol );
}


/**
 @brief 
 */
bool CFarmServerConnector::Start( const std::string &ip, const int port )
{
	if (!GetConnector())
		return false;

	GetConnector()->Start(ip, port);
	return true;
}


/**
 @brief P2P, Input_lin, Output_link 按眉甫 积己茄促.
 */
void	CFarmServerConnector::CreateLink()
{
	// Input Link 积己
	BOOST_FOREACH(auto &bindSubSvrType, m_Config.inputLink)
	{
		CreateSubController( CLIENT, true, m_Config.svrType, bindSubSvrType );
	}

	// Output Link 积己
	BOOST_FOREACH(auto &bindSubSvrType, m_Config.outputLink)
	{
		CreateSubController( SERVER, true, m_Config.svrType, bindSubSvrType );
	}

	// P2P Client Link 积己
	BOOST_FOREACH(auto &bindSubSvrType, m_Config.p2pS)
	{
		CreateSubController( CLIENT, true, m_Config.svrType, bindSubSvrType );
	}

	// 扁夯 辑滚 积己 
	CreateSubController( SERVER, false, m_Config.svrType,  "client" );

}


/**
 @brief ConnectLink
 */
void	CFarmServerConnector::ConnectLink()
{
	// Input Link 积己
	BOOST_FOREACH(auto &bindSubSvrType, m_Config.inputLink)
	{
		ConnectSubController( CLIENT, true, m_Config.svrType, bindSubSvrType );
	}

	// Output Link 积己
	BOOST_FOREACH(auto &bindSubSvrType, m_Config.outputLink)
	{
		ConnectSubController( SERVER, true, m_Config.svrType, bindSubSvrType );
	}

	// P2P Client Link 积己
	BOOST_FOREACH(auto &bindSubSvrType, m_Config.p2pS)
	{
		ConnectSubController( CLIENT, true, m_Config.svrType, bindSubSvrType );
	}
}


/**
 @brief NetGroupController 积己
 @param IsInnerBind : true: to Bind Inner Space Network
									 false: to Bind Outer Space Network
 */
bool	CFarmServerConnector::CreateSubController( SERVICE_TYPE serviceType, bool IsInnerBind,
	const std::string &connectSubSvrType, const std::string &bindSubSvrType )
{
	if (connectSubSvrType == bindSubSvrType)
		return false;

	NetGroupControllerPtr ptr = CMultiNetwork::Get()->GetController(bindSubSvrType);
	if (ptr)
		return true;

	CNetGroupController *pctrl = new CNetGroupController(serviceType, connectSubSvrType, bindSubSvrType);
	if (!CMultiNetwork::Get()->AddController(pctrl))
	{
		clog::Error(log::ERROR_CRITICAL, "Not Create NetGroupController !!" );
		delete pctrl;
		return false;
	}

	return true;
}


/**
@brief ConnectSubController
*/
void	CFarmServerConnector::ConnectSubController(SERVICE_TYPE serviceType, bool IsInnerBind,
	const std::string &connectSubSvrType, const std::string &bindSubSvrType )
{
	NetGroupControllerPtr ptr = CMultiNetwork::Get()->GetController(bindSubSvrType);
	if (!ptr)
		return;

	if (ptr->IsConnect())
		return;

	ptr->SetTryConnect();
	if (CLIENT == serviceType)
	{
		m_Protocol.ReqServerInfoList( SERVER_NETID, SEND_T, connectSubSvrType,
			bindSubSvrType );
	}
	else if (SERVER == serviceType)
	{
		if (IsInnerBind)
			m_Protocol.ReqToBindInnerPort( SERVER_NETID, SEND_T, bindSubSvrType );
		else
			m_Protocol.ReqToBindOuterPort( SERVER_NETID, SEND_T, bindSubSvrType );
	}
}


/**
 @brief Login 夸没
 */
void CFarmServerConnector::OnConnect(CNetEvent &event)
{
	m_Protocol.ReqSubServerLogin( SERVER_NETID, SEND_T, m_Config.svrType );
}


/**
 @brief 
 */
void CFarmServerConnector::AckSubServerLogin(netid senderId, const error::ERROR_CODE &errorCode)
{
	if (errorCode == error::ERR_SUCCESS)
	{
		std::vector<std::string> v;
		std::copy( m_Config.p2pC.begin(), m_Config.p2pC.end(), std::back_inserter(v) );
		std::copy( m_Config.p2pS.begin(), m_Config.p2pS.end(), std::back_inserter(v) );
		m_Protocol.SendSubServerP2PLink( SERVER_NETID, SEND_T, v );
		m_Protocol.SendSubServerInputLink( SERVER_NETID, SEND_T, m_Config.inputLink );
		m_Protocol.SendSubServerOutputLink( SERVER_NETID, SEND_T, m_Config.outputLink );
	}
}


/**
 @brief 
 */
void CFarmServerConnector::AckSendSubServerP2PLink(netid senderId, const error::ERROR_CODE &errorCode)
{
	if (!m_IsDetectedSendConfig)
		m_IsDetectedSendConfig = (errorCode != error::ERR_SUCCESS);

	if (errorCode != error::ERR_SUCCESS)
		clog::Error( clog::ERROR_CRITICAL, "P2P Link Error!!" );
}

/**
 @brief 
 */
void CFarmServerConnector::AckSendSubServerInputLink(netid senderId, const error::ERROR_CODE &errorCode)
{
	if (!m_IsDetectedSendConfig)
		m_IsDetectedSendConfig = (errorCode != error::ERR_SUCCESS);

	if (errorCode != error::ERR_SUCCESS)
		clog::Error( clog::ERROR_CRITICAL, "P2P Link Error!!" );
}


/**
 @brief 
 */
void CFarmServerConnector::AckSendSubServerOutputLink(netid senderId, const error::ERROR_CODE &errorCode)
{
	if (!m_IsDetectedSendConfig)
		m_IsDetectedSendConfig = (errorCode != error::ERR_SUCCESS);

	if (m_IsDetectedSendConfig)
	{
		clog::Error( clog::ERROR_CRITICAL, "AckSendSubServerOutputLink Error"  );
		return;
	}

	ConnectLink();
	//MakeupInputOutputLink();
	//MakeupP2PLink();

	//// 磊脚狼 辑滚按眉 积己
	//std::string bindSubSvrType;
	//if (m_Config.inputLink.size() > 0 || m_Config.outputLink.size() > 0)
	//	bindSubSvrType = "client";		
	//else if (m_Config.p2pC.size() > 0)
	//	bindSubSvrType = "p2p";
	//else
	//	bindSubSvrType = "none";
	//CreateSubController( SERVER, false, m_Config.svrType, bindSubSvrType );
}


/**
 @brief 
 */
void CFarmServerConnector::AckServerInfoList(netid senderId, const error::ERROR_CODE &errorCode, 
	const std::string &clientSvrType, const std::string &serverSvrType, const std::vector<SHostInfo> &v)
{
	if (errorCode != error::ERR_SUCCESS)
	{
		clog::Error( clog::ERROR_CRITICAL, 
			"AckServerInfoList Error!!, not found port number  clientSvrType = %s, serverSvrType = %s", 
			clientSvrType.c_str(), serverSvrType.c_str() );
		return;
	}

	// Connect Client
	NetGroupControllerPtr ptr = CMultiNetwork::Get()->GetController(serverSvrType);
	if (!ptr)
	{
		clog::Error( clog::ERROR_CRITICAL, 
			"AckServerInfoList Error!!, not found controller serverSvrType = %s", 
			serverSvrType.c_str() );
		return;
	}
	
	if (!ptr->Start(v))
	{
		clog::Error( clog::ERROR_CRITICAL, "NetGroupController Start Error!!" );
	}
}


/**
 @brief 
 */
void CFarmServerConnector::AckToBindOuterPort(netid senderId, const error::ERROR_CODE &errorCode, 
	const std::string &bindSubServerSvrType, const int &port)
{
	if (errorCode != error::ERR_SUCCESS)
	{
		clog::Error( clog::ERROR_CRITICAL, 
			"AckToBindOuterPort Error!!, not found port number  bindSubServerSvrType = %s", 
			bindSubServerSvrType.c_str() );
		return;
	}

	NetGroupControllerPtr ptr = CMultiNetwork::Get()->GetController(bindSubServerSvrType);
	if (!ptr)
	{
		clog::Error( clog::ERROR_CRITICAL, 
			"AckToBindOuterPort Error!!, not found controller bindSubServerSvrType = %s", 
			bindSubServerSvrType.c_str() );
		return;
	}

	// Server Bind
	if (!ptr->Start( "", port))
	{
		clog::Error( clog::ERROR_CRITICAL, "NetGroupController Start Error!!" );
	}
}


/**
 @brief 
 */
void CFarmServerConnector::AckToBindInnerPort(netid senderId, const error::ERROR_CODE &errorCode, 
	const std::string &bindSubServerSvrType, const int &port)
{
	if (errorCode != error::ERR_SUCCESS)
	{
		clog::Error( clog::ERROR_CRITICAL, 
			"AckToBindInnerPort Error!!, not found port number  bindSubServerSvrType = %s", 
			bindSubServerSvrType.c_str() );
		return;
	}

	NetGroupControllerPtr ptr = CMultiNetwork::Get()->GetController(bindSubServerSvrType);
	if (!ptr)
	{
		clog::Error( clog::ERROR_CRITICAL, 
			"AckToBindInnerPort Error!!, not found controller bindSubServerSvrType = %s", 
			bindSubServerSvrType.c_str() );
		return;
	}

	// Server Bind
	if (!ptr->Start( "", port))
	{
		clog::Error( clog::ERROR_CRITICAL, "NetGroupController Start Error!!" );
	}
}


/**
 @brief 
 */
void CFarmServerConnector::AckSubServerBindComplete(netid senderId, const error::ERROR_CODE &errorCode, 
	const std::string &subServerSvrType)
{

}


/**
 @brief 
 */
void CFarmServerConnector::AckSubClientConnectComplete(netid senderId, const error::ERROR_CODE &errorCode, 
	const std::string &subClientSvrType)
{

}

