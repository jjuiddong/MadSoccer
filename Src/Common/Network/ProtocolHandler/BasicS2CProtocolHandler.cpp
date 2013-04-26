
#include "stdafx.h"
#include "BasicS2CProtocolHandler.h"
#include "../Controller/P2PClient.h"

using namespace network;


CBasicS2CProtocolHandler::CBasicS2CProtocolHandler( CClient &client ) :
	m_Client(client)
{
	client.RegisterProtocol(&m_BasicProtocol);
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

