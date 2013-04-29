
#include "stdafx.h"
#include "FarmServerConnector.h"
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
	NETEVENT_CONNECT(EVT_CONNECT, CFarmServerConnector, CFarmServerConnector::OnConnect );
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
	BOOST_FOREACH(auto &bindSubSvrType, m_Config.p2pC)
	{
		CreateSubController( CLIENT, true, m_Config.svrType, bindSubSvrType );
	}

	// P2P Server Link 积己
	if (m_Config.p2pS.size() > 0)
	{
		CreateSubController( SERVER, true, m_Config.svrType,  "p2p" );
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
	BOOST_FOREACH(auto &bindSubSvrType, m_Config.p2pC)
	{
		ConnectSubController( CLIENT, true, m_Config.svrType, bindSubSvrType );
	}

	// P2P Server Link 积己
	if (m_Config.p2pS.size() > 0)
	{
		ConnectSubController( SERVER, true, m_Config.svrType,  "p2p" );
	}

	// 扁夯 辑滚 Bind
	ConnectSubController( SERVER, false, m_Config.svrType,  "client" );
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

	CMultiPlug *pctrl = new CMultiPlug(serviceType, connectSubSvrType, bindSubSvrType);
	if (!CMultiNetwork::Get()->AddController(pctrl))
	{
		clog::Error(log::ERROR_CRITICAL, "Not Create NetGroupController !!" );
		delete pctrl;
		return false;
	}

	NETEVENT_CONNECT_TO( pctrl, this, EVT_CONNECT, CFarmServerConnector, CFarmServerConnector::OnConnectLink );
	NETEVENT_CONNECT_TO( pctrl, this, EVT_DISCONNECT, CFarmServerConnector, CFarmServerConnector::OnDisconnectLink );

	return true;
}


/**
@brief ConnectSubController
*/
void	CFarmServerConnector::ConnectSubController( SERVICE_TYPE serviceType, bool IsInnerBind,
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
@brief OnConnectLink
 */
void CFarmServerConnector::OnConnectLink(CNetEvent &event)
{
	CMultiPlug *pctrl = dynamic_cast<CMultiPlug*>(event.GetHandler().Get());
	if (!pctrl)
		return;

	if (pctrl->GetServiceType() == CLIENT)
		m_Protocol.ReqSubClientConnectComplete( SERVER_NETID, SEND_T, pctrl->GetConnectSvrType() );
	else if (pctrl->GetServiceType() == SERVER)
		m_Protocol.ReqSubServerBindComplete( SERVER_NETID, SEND_T, pctrl->GetConnectSvrType() );
}


/**
@brief OnDisconnectLink
 */
void CFarmServerConnector::OnDisconnectLink(CNetEvent &event)
{
	//clog::ErrorMsg( "Err!!! Not Connect or Not Bind NetGroupController" );
}


/**
 @brief 
 */
bool CFarmServerConnector::AckSubServerLogin(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode)
{
	if (errorCode == error::ERR_SUCCESS)
	{
		m_Protocol.SendSubServerP2PCLink( SERVER_NETID, SEND_T, m_Config.p2pC );
		m_Protocol.SendSubServerP2PSLink( SERVER_NETID, SEND_T, m_Config.p2pS );
		m_Protocol.SendSubServerInputLink( SERVER_NETID, SEND_T, m_Config.inputLink );
		m_Protocol.SendSubServerOutputLink( SERVER_NETID, SEND_T, m_Config.outputLink );
		return true;
	}
	else
	{
		clog::Error( clog::ERROR_CRITICAL, "AckSubServerLogin Error!! errorCode= %d, svrType = %s", errorCode, m_Config.svrType.c_str() );
		clog::ErrorMsg( common::format( "AckSubServerLogin Error!! errorCode= %d, svrType = %s", errorCode, m_Config.svrType.c_str()) );
		return false;
	}
}


/**
 @brief AckSendSubServerP2PCLink
 */
bool CFarmServerConnector::AckSendSubServerP2PCLink(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode)
{
	if (!m_IsDetectedSendConfig)
		m_IsDetectedSendConfig = (errorCode != error::ERR_SUCCESS);

	if (errorCode != error::ERR_SUCCESS)
		clog::Error( clog::ERROR_CRITICAL, "P2PC Link Error!!" );

	return true;
}


/**
 @brief AckSendSubServerP2PSLink
 */
bool CFarmServerConnector::AckSendSubServerP2PSLink(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode)
{
	if (!m_IsDetectedSendConfig)
		m_IsDetectedSendConfig = (errorCode != error::ERR_SUCCESS);

	if (errorCode != error::ERR_SUCCESS)
		clog::Error( clog::ERROR_CRITICAL, "P2PS Link Error!!" );

	return true;
}

/**
 @brief 
 */
bool CFarmServerConnector::AckSendSubServerInputLink(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode)
{
	if (!m_IsDetectedSendConfig)
		m_IsDetectedSendConfig = (errorCode != error::ERR_SUCCESS);

	if (errorCode != error::ERR_SUCCESS)
		clog::Error( clog::ERROR_CRITICAL, "P2P Link Error!!" );

	return true;
}


/**
 @brief 
 */
bool CFarmServerConnector::AckSendSubServerOutputLink(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode)
{
	if (!m_IsDetectedSendConfig)
		m_IsDetectedSendConfig = (errorCode != error::ERR_SUCCESS);

	if (m_IsDetectedSendConfig)
	{
		clog::Error( clog::ERROR_CRITICAL, "AckSendSubServerOutputLink Error"  );
		return false;
	}

	ConnectLink();
	return true;
}


/**
 @brief 
 */
bool CFarmServerConnector::AckServerInfoList(IProtocolDispatcher &dispatcher, netid senderId, 
	const error::ERROR_CODE &errorCode, 
	const std::string &clientSvrType, const std::string &serverSvrType, const std::vector<SHostInfo> &v)
{
	if (errorCode != error::ERR_SUCCESS)
	{
		clog::Error( clog::ERROR_CRITICAL, 
			"AckServerInfoList Error!!, not found port number  clientSvrType = %s, serverSvrType = %s", 
			clientSvrType.c_str(), serverSvrType.c_str() );
		return true;
	}

	// Connect Client
	NetGroupControllerPtr ptr = CMultiNetwork::Get()->GetController(serverSvrType);
	if (!ptr)
	{
		clog::Error( clog::ERROR_CRITICAL, 
			"AckServerInfoList Error!!, not found controller serverSvrType = %s", 
			serverSvrType.c_str() );
		return true;
	}
	
	if (!ptr->Start(v))
	{
		clog::Error( clog::ERROR_CRITICAL, "NetGroupController Start Error!!" );
		return false;
	}
	return true;
}


/**
 @brief 
 */
bool CFarmServerConnector::AckToBindOuterPort(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode, 
	const std::string &bindSubServerSvrType, const int &port)
{
	if (errorCode != error::ERR_SUCCESS)
	{
		clog::Error( clog::ERROR_CRITICAL, 
			"AckToBindOuterPort Error!!, not found port number  bindSubServerSvrType = %s", 
			bindSubServerSvrType.c_str() );
		return false;
	}

	NetGroupControllerPtr ptr = CMultiNetwork::Get()->GetController(bindSubServerSvrType);
	if (!ptr)
	{
		clog::Error( clog::ERROR_CRITICAL, 
			"AckToBindOuterPort Error!!, not found controller bindSubServerSvrType = %s", 
			bindSubServerSvrType.c_str() );
		return false;
	}

	// Server Bind
	if (!ptr->Start( "", port))
	{
		clog::Error( clog::ERROR_CRITICAL, "NetGroupController Start Error!!" );
		return false;
	}
	return true;
}


/**
 @brief 
 */
bool CFarmServerConnector::AckToBindInnerPort(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode, 
	const std::string &bindSubServerSvrType, const int &port)
{
	if (errorCode != error::ERR_SUCCESS)
	{
		clog::Error( clog::ERROR_CRITICAL, 
			"AckToBindInnerPort Error!!, not found port number  bindSubServerSvrType = %s", 
			bindSubServerSvrType.c_str() );
		return false;
	}

	NetGroupControllerPtr ptr = CMultiNetwork::Get()->GetController(bindSubServerSvrType);
	if (!ptr)
	{
		clog::Error( clog::ERROR_CRITICAL, 
			"AckToBindInnerPort Error!!, not found controller bindSubServerSvrType = %s", 
			bindSubServerSvrType.c_str() );
		return false;
	}

	// Server Bind
	if (!ptr->Start( "", port))
	{
		clog::Error( clog::ERROR_CRITICAL, "NetGroupController Start Error!!" );
		return false;
	}
	return true;
}


/**
 @brief 
 */
bool CFarmServerConnector::AckSubServerBindComplete(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode, 
	const std::string &subServerSvrType)
{

	return true;
}


/**
 @brief 
 */
bool CFarmServerConnector::AckSubClientConnectComplete(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode, 
	const std::string &subClientSvrType)
{

	return true;
}


/**
 @brief 
 */
bool CFarmServerConnector::BindSubServer(IProtocolDispatcher &dispatcher, netid senderId, const std::string &bindSubSvrType, 
	const std::string &ip, const int &port)
{
	NetGroupControllerPtr ptr = CMultiNetwork::Get()->GetController(bindSubSvrType);	
	if (!ptr)
	{
		clog::Error( clog::ERROR_CRITICAL, "BindSubServer Error!! not found bindSubSvr : %s", bindSubSvrType.c_str() );
		return false;
	}

	if (!ptr->Start(ip, port))
	{
		clog::Error( clog::ERROR_CRITICAL, "BindSubServer Start Error!! ip=%s, port=%d", ip.c_str(), port );
		return false;
	}
	return true;
}
