
#include "stdafx.h"
#include "NetGroupController.h"
#include "../Controller/CoreClient.h"
#include "../Controller/P2PClient.h"
#include "../Controller/ServerBasic.h"
#include "../Controller/NetController.h"

using namespace network;
using namespace network::multinetwork;


CNetGroupController::CNetGroupController( SERVICE_TYPE type, const std::string &svrType,
	const std::string &connectSvrType ) :
	CNetConnector(SERVICE_EXCLUSIVE_THREAD)
,	m_State(WAIT)
,	m_ServiceType(type)
,	m_svrType(svrType)
,	m_connectSvrType(connectSvrType)
,	m_pClient(NULL)
,	m_pServer(NULL)
,	m_pP2p(NULL)
,	m_pRemoteClientFactory(NULL)
,	m_pGroupFactory(NULL)
{

}

CNetGroupController::~CNetGroupController()
{
	SAFE_DELETE(m_pClient);
	SAFE_DELETE(m_pServer);
	SAFE_DELETE(m_pP2p);
	SAFE_DELETE(m_pRemoteClientFactory);
	SAFE_DELETE(m_pGroupFactory);
}


/**
 @brief 
 */
bool	CNetGroupController::Start(const std::string &ip, const int port)
{
	return Connect(m_ServiceType, ip, port);
}


/**
 @brief 클라이언트로 서버에 접속할 때, 접속할 서버가 한개 이상일 때, 
 이 함수가 쓰인다.
 */
bool	CNetGroupController::Start(const std::vector<SHostInfo> &v)
{
	if (CLIENT != m_ServiceType)
		return false;
	
	BOOST_FOREACH(auto &info, v)
	{
		Connect( m_ServiceType, info.ip, info.portnum );
	}
	return true;
}


/**
 @brief 
 */
void	CNetGroupController::Stop()
{
	if (m_pServer)
		m_pServer->Stop();
	if (m_pP2p)
		m_pP2p->Stop();
	BOOST_FOREACH(auto &client, m_Clients.m_Seq)
	{
		if (client) client->Stop();
	}
}


/**
 @brief 
 */
bool	CNetGroupController::Send(netid netId, const SEND_FLAG flag, const CPacket &packet)
{

	return true;
}


/**
 @brief 
 */
bool	CNetGroupController::SendAll(const CPacket &packet) 
{

	return true;
}


/**
 @brief 
 */
bool	CNetGroupController::Connect( SERVICE_TYPE type, const std::string &ip, const int port )
{
	switch (type)
	{
	case SERVER:
		{
			m_Ip = "localhost";
			m_Port = port;

			if (m_pServer)
			{
				m_pServer->Disconnect();
			}
			else
			{
				m_pServer = new CServerBasic(SERVICE_EXCLUSIVE_THREAD);
				if (m_pRemoteClientFactory)
					m_pServer->SetRemoteClientFactory(m_pRemoteClientFactory->Clone());
			}
			CNetController::Get()->StartServer(port, m_pServer);
		}
		break;

	case CLIENT:
		{
			CCoreClient *pClient = new CCoreClient(SERVICE_SEPERATE_THREAD);
			CNetController::Get()->StartCoreClient(ip, port, pClient);
			m_Clients.insert( Clients::value_type(pClient->GetNetId(), pClient) );

			m_Ip = ip;
			m_Port = port;
		}
		break;
	}

	return true;
}


/**
 @brief 
 */
void	CNetGroupController::SetRemoteClientFactory( IRemoteClientFactory *ptr )
{
	SAFE_DELETE(m_pRemoteClientFactory);
	m_pRemoteClientFactory = ptr;
}


/**
 @brief 
 */
void	CNetGroupController::SetGroupFactory( IGroupFactory *ptr )
{
	SAFE_DELETE(m_pGroupFactory);
	m_pGroupFactory = ptr;
}
