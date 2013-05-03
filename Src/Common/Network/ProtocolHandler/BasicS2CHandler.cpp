
#include "stdafx.h"
#include "BasicS2CHandler.h"
#include "../Controller/P2PClient.h"
#include "../Service/Client.h"

using namespace network;


CBasicS2CHandler::CBasicS2CHandler( CClient &client ) :
	m_Client(client)
,	m_ClientState(CLIENT_END_MOVE)
,	m_IsMoveToServer(false)
{
	client.RegisterProtocol(&m_BasicProtocol);

	NETEVENT_CONNECT_TO(&client, this, EVT_CONNECT, CBasicS2CHandler, CBasicS2CHandler::OnConnectClient);
	NETEVENT_CONNECT_TO(&client, this, EVT_DISCONNECT, CBasicS2CHandler, CBasicS2CHandler::OnDisconnectClient);
}

CBasicS2CHandler::~CBasicS2CHandler()
{

}


/**
 @brief AckLogIn
 */
bool CBasicS2CHandler::AckLogIn(IProtocolDispatcher &dispatcher, netid senderId, const error::ERROR_CODE &errorCode, 
	const std::string &id, const certify_key &c_key)
{ 
	if (error::ERR_SUCCESS == errorCode)
	{
		if (CLIENT_CONNECT == m_ClientState)
		{
			m_ClientState = CLIENT_END_MOVE;
		}
		else
		{
			m_Client.SetName(id);
			m_Client.SetCertifyKey(c_key);
		}
	}
	return true; 
}


/**
 @brief Acknowlege packet of RequestP2PConnect 
 */
bool CBasicS2CHandler::AckP2PConnect(
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
bool CBasicS2CHandler::AckMoveToServer(IProtocolDispatcher &dispatcher, netid senderId, 
	const error::ERROR_CODE &errorCode, const std::string &serverName, const std::string &ip, const int &port)
{
	if (error::ERR_SUCCESS != errorCode)
		return false;

	m_Client.Stop();

	m_ClientState = CLIENT_BEGIN_MOVE;
	m_IsMoveToServer = true;
	m_ToServerIp = ip;
	m_ToServerPort = port;
	return true;
}


/**
 @brief OnDisconnectClient
 */
void	CBasicS2CHandler::OnDisconnectClient(CNetEvent &event)
{
	if (CLIENT_BEGIN_MOVE == m_ClientState)
	{
		m_ClientState = CLIENT_CLOSE;
		StartClient(m_ToServerIp, m_ToServerPort, &m_Client);
		m_IsMoveToServer = false;
	}	
}


/**
 @brief OnConnectClient
 */
void	CBasicS2CHandler::OnConnectClient(CNetEvent &event)
{
	if (CLIENT_CLOSE == m_ClientState)
	{
		m_ClientState = CLIENT_CONNECT;
		clog::Log( clog::LOG_F_N_O, clog::LOG_PACKET, 1, "MoveToServer ReqLogin id=%s, c_key=%d", 
			m_Client.GetName().c_str(), m_Client.GetCertifyKey() );
		m_BasicProtocol.ReqLogIn( SERVER_NETID, SEND_T, m_Client.GetName(), "", m_Client.GetCertifyKey());
	}
}
