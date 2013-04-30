
#include "stdafx.h"
#include "CertifyServer.h"

#include "NetProtocol/src/certify_Protocol.cpp"
#include "NetProtocol/src/certify_ProtocolListener.cpp"

using namespace network;

CCertifyServer::CCertifyServer()
{

}

CCertifyServer::~CCertifyServer()
{

}


/**
 @brief OnConnectNetGroupController
 */
void	CCertifyServer::OnConnectMultiPlug()
{
	CServer::OnConnectMultiPlug();

	AddProtocolListener(this);
	RegisterProtocol( &m_Protocol );
}


/**
 @brief ReqUserId
 */
bool CCertifyServer::ReqUserId(IProtocolDispatcher &dispatcher, netid senderId, const std::string &id, const std::string &passwd)
{
	m_Protocol.AckUserId( senderId, SEND_T, error::ERR_SUCCESS, id, common::GenerateId() );

	return true;
}
