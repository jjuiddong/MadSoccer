
#include "stdafx.h"
#include "BasicS2CProtocolHandler.h"
#include "../Controller/P2PClient.h"
#include "../Service/Client.h"

using namespace network;


CBasicS2CProtocolHandler::CBasicS2CProtocolHandler( CClient &client ) :
	m_Client(client)
,	m_IsMoveToServer(false)
{
	client.RegisterProtocol(&m_BasicProtocol);

	NETEVENT_CONNECT_TO(&client, this, EVT_DISCONNECT, CBasicS2CProtocolHandler, CBasicS2CProtocolHandler::OnDisconnectClient);
}

CBasicS2CProtocolHandler::~CBasicS2CProtocolHandler()
{

}


/**
 @brief Acknowlege packet of RequestP2PConnect 
 */
bool CBasicS2CProtocolHandler::AckP2PConnect(
	IProtocolDispatcher &dispatcher, netid senderId, const network::error::ERROR_CODE &errorCode, 
	const network::P2P_STATE &state, const std::string &ip, const int &port)
{
	if (errorCode != error::ERR_SUCCESS)
		return false; // todo: error process

	if (!m_Client.m_pP2p)
		return false; // error!!
	
	bool result = false;
	if (state == P2P_HOST)
	{
		result = m_Client.m_pP2p->Bind( port );
	}
	else if(state == P2P_CLIENT)
	{
		result = m_Client.m_pP2p->Connect(ip, port);
	}

	m_BasicProtocol.ReqP2PConnectTryResult(SERVER_NETID, SEND_TARGET, result);
	return true;
}


/**
 @brief AckMoveToServer
 */
bool CBasicS2CProtocolHandler::AckMoveToServer(IProtocolDispatcher &dispatcher, netid senderId, 
	const error::ERROR_CODE &errorCode, const std::string &serverName, const std::string &ip, const int &port)
{
	if (error::ERR_SUCCESS != errorCode)
		return false;

	m_Client.Stop();

	m_IsMoveToServer = true;
	m_ToServerIp = ip;
	m_ToServerPort = port;
	return true;
}


/**
 @brief OnDisconnectClient
 */
void	CBasicS2CProtocolHandler::OnDisconnectClient(CNetEvent &event)
{
	if (m_IsMoveToServer)
	{
		StartClient(m_ToServerIp, m_ToServerPort, &m_Client);
		m_IsMoveToServer = false;
	}	
}
