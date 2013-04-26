
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
	CNetConnector(SERVICE_SEPERATE_THREAD)
,	m_State(WAIT)
,	m_ServiceType(type)
,	m_svrType(svrType)
,	m_connectSvrType(connectSvrType)
,	m_pClient(NULL)
,	m_pServer(NULL)
,	m_pP2p(NULL)
,	m_pSessionFactory(NULL)
,	m_pGroupFactory(NULL)
{
	if (SERVER == type)
	{
		m_pServer = new CServerBasic( SERVICE_SEPERATE_THREAD );
		NETEVENT_CONNECT_TO( m_pServer, this, EVT_LISTEN, CNetGroupController, CNetGroupController::OnConnect );
		NETEVENT_CONNECT_TO( m_pServer, this, EVT_CONNECT, CNetGroupController, CNetGroupController::OnConnect );
		NETEVENT_CONNECT_TO( m_pServer, this, EVT_DISCONNECT, CNetGroupController, CNetGroupController::OnDisconnect );
	}
	
}

CNetGroupController::~CNetGroupController()
{
	SAFE_DELETE(m_pClient);
	SAFE_DELETE(m_pServer);
	SAFE_DELETE(m_pP2p);
	SAFE_DELETE(m_pSessionFactory);
	SAFE_DELETE(m_pGroupFactory);
	BOOST_FOREACH(auto &client, m_Clients.m_Seq)
	{
		SAFE_DELETE(client);
	}
	m_Clients.clear();
	BOOST_FOREACH(auto &client, m_RemoveClients.m_Seq)
	{
		SAFE_DELETE(client);
	}
	m_RemoveClients.clear();
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
 @brief Send
 */
bool	CNetGroupController::Send(netid netId, const SEND_FLAG flag, const CPacket &packet)
{
	switch (m_ServiceType)
	{
	case CLIENT:
		{
			BOOST_FOREACH(auto &client, m_Clients.m_Seq)
			{
				if (client && client->IsConnect())
					return client->Send(netId, flag, packet);
			}
		}
		break;
	case SERVER:
		if (m_pServer)
			return m_pServer->Send(netId, flag, packet);
		break;
	}
	return false;
}


/**
 @brief SendAll
 */
bool	CNetGroupController::SendAll(const CPacket &packet) 
{
	switch (m_ServiceType)
	{
	case CLIENT:
		BOOST_FOREACH(auto &client, m_Clients.m_Seq)
		{
			if (client)
				return client->SendAll(packet);
		}
		break;
	case SERVER:
		if (m_pServer)
			return m_pServer->SendAll(packet);
		break;
	}
	return false;
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
			if (m_pServer->IsServerOn())
				m_pServer->Disconnect();
			CNetController::Get()->StartServer(port, m_pServer);
		}
		break;

	case CLIENT:
		{
			m_Ip = ip;
			m_Port = port;

			CCoreClient *pClient = new CCoreClient(SERVICE_SEPERATE_THREAD);
			NETEVENT_CONNECT_TO( pClient, this, EVT_CONNECT, CNetGroupController, CNetGroupController::OnConnect );
			NETEVENT_CONNECT_TO( pClient, this, EVT_DISCONNECT, CNetGroupController, CNetGroupController::OnDisconnect );
			BOOST_FOREACH(auto &protocol, GetProtocolListeners())
			{
				pClient->AddProtocolListener(protocol);
			}
			m_Clients.insert( CoreClients_::value_type(pClient->GetNetId(), pClient) );

			CNetController::Get()->StartCoreClient(ip, port, pClient);
		}
		break;
	}

	return true;
}


/**
 @brief 
 */
void	CNetGroupController::SetSessionFactory( ISessionFactory *ptr )
{
	SAFE_DELETE(m_pSessionFactory);
	m_pSessionFactory = ptr;
	if (m_pServer)
		m_pServer->SetSessionFactory(m_pSessionFactory->Clone());
}


/**
 @brief SetGroupFactory
 */
void	CNetGroupController::SetGroupFactory( IGroupFactory *ptr )
{
	SAFE_DELETE(m_pGroupFactory);
	m_pGroupFactory = ptr;
	if (m_pServer && ptr)
		m_pServer->SetGroupFactory( ptr->Clone() );
}


/**
 @brief OnConnect
 */
void	CNetGroupController::OnConnect( CNetEvent &event )
{
	m_State = RUN;
	SearchEventTable(CNetEvent(EVT_CONNECT, this));// Event Propagate
}


/**
 @brief OnDisconnect
 */
void	CNetGroupController::OnDisconnect( CNetEvent &event )
{
	if (m_ServiceType == SERVER)
		m_State = END;
	SearchEventTable(CNetEvent(EVT_DISCONNECT, this)); // Event Propagate

	// disconnect 된 client 제거
	if (CLIENT == m_ServiceType)
	{
		auto it = m_Clients.find(event.GetHandler()->GetNetId());
		if (m_Clients.end() == it)
			return;

		it->second->Stop();
		m_Clients.remove( event.GetHandler()->GetNetId() );

		// 제거 clients 로 옮겨져서, 나중에 지워진다.
		m_RemoveClients.insert( CoreClients_::value_type(event.GetHandler()->GetNetId(), 
			(CCoreClient*)event.GetHandler().Get() ) );
	}
}


/**
 @brief AddProtocolListener
 */
bool	CNetGroupController::AddProtocolListener(ProtocolListenerPtr pListener)
{
	CNetConnector::AddProtocolListener(pListener);
	if (m_pServer)
		m_pServer->AddProtocolListener(pListener);
	BOOST_FOREACH(auto &client, m_Clients.m_Seq)
	{
		client->AddProtocolListener(pListener);
	}
	return true;
}


/**
 @brief RemoveProtocolListener
 */
bool	CNetGroupController::RemoveProtocolListener(ProtocolListenerPtr pListener)
{
	CNetConnector::RemoveProtocolListener(pListener);
	if (m_pServer)
		m_pServer->RemoveProtocolListener(pListener);
	BOOST_FOREACH(auto &client, m_Clients.m_Seq)
	{
		client->RemoveProtocolListener(pListener);
	}
	return true;
}
