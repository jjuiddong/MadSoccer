
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
void	CCertifyServer::OnConnectNetGroupController()
{
	CServer::OnConnectNetGroupController();

	AddProtocolListener(this);
	RegisterProtocol( &m_Protocol );
}


/**
 @brief ReqUserId
 */
void CCertifyServer::ReqUserId(netid senderId, const std::string &id, const std::string &passwd)
{
	m_Protocol.AckUserId( senderId, SEND_T, error::ERR_SUCCESS, id, common::GenerateId() );

}
