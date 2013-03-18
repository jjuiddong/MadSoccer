
#include "stdafx.h"
#include "BasicS2CProtocolHandler.h"
#include "../Controller/P2PClient.h"

using namespace network;


CBasicS2CProtocolHandler::CBasicS2CProtocolHandler( CClient &client ) :
	m_Client(client)
{
	m_BasicProtocol.SetNetConnector(&client);
}

CBasicS2CProtocolHandler::~CBasicS2CProtocolHandler()
{
}


/**
 @brief 
 */
void CBasicS2CProtocolHandler::AckP2PConnect(
	netid senderId, const int &errorCode, const network::P2P_STATE &state, const std::string &ip, const int &port)
{
	if (errorCode == error::ERR_SUCCESS)
	{
		if (state == P2P_HOST)
		{
			if (m_Client.m_pP2p)
				m_Client.m_pP2p->Bind( port );
		}
		else if(state == P2P_CLIENT)
		{
			if (m_Client.m_pP2p)
				m_Client.m_pP2p->Connect(ip, port);
		}
	}
	else
	{

	}
}


/**
 @brief 
 */
void CBasicS2CProtocolHandler::AckP2PHostCreate(netid senderId, const int &errorCode)
{

}

