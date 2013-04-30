
#include "stdafx.h"
#include "MultiPlug.h"
#include "../Controller/CoreClient.h"
#include "../Controller/P2PClient.h"
#include "../Controller/ServerBasic.h"
#include "../Controller/NetController.h"

using namespace network;
using namespace network::multinetwork;


CMultiPlug::CMultiPlug( SERVICE_TYPE type, const std::string &svrType,
	const std::string &connectSvrType ) :
	CPlug(SERVICE_SEPERATE_THREAD)
,	m_State(WAIT)
,	m_ServiceType(type)
,	m_svrType(svrType)
,	m_connectSvrType(connectSvrType)
//,	m_pClient(NULL)
,	m_pServer(NULL)
,	m_pP2p(NULL)
,	m_pSessionFactory(NULL)
,	m_pGroupFactory(NULL)
{
	if (SERVER == type)
	{
		m_pServer = new CServerBasic( SERVICE_SEPERATE_THREAD );
		NETEVENT_CONNECT_TO( m_pServer, this, EVT_LISTEN, CMultiPlug, CMultiPlug::OnConnect );
		NETEVENT_CONNECT_TO( m_pServer, this, EVT_CONNECT, CMultiPlug, CMultiPlug::OnConnect );
		NETEVENT_CONNECT_TO( m_pServer, this, EVT_DISCONNECT, CMultiPlug, CMultiPlug::OnDisconnect );
		AddChild(m_pServer);
	}
	
}

CMultiPlug::~CMultiPlug()
{
	//SAFE_DELETE(m_pClient);
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
bool	CMultiPlug::Start(const std::string &ip, const int port)
{
	return Connect(m_ServiceType, ip, port);
}


/**
 @brief Ŭ���̾�Ʈ�� ������ ������ ��, ������ ������ �Ѱ� �̻��� ��, 
 �� �Լ��� ���δ�.
 */
bool	CMultiPlug::Start(const std::vector<SHostInfo> &v)
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
void	CMultiPlug::Stop()
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
bool	CMultiPlug::Send(netid netId, const SEND_FLAG flag, const CPacket &packet)
{
	switch (m_ServiceType)
	{
	case CLIENT:
		{
			if (SERVER_NETID == netId)
			{
				SendAll(packet);
			}
			else
			{
				CoreClientPtr pClient = GetClientFromServerNetId(netId);
				if (pClient)
					pClient->Send(netId, flag, packet);
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
bool	CMultiPlug::SendAll(const CPacket &packet) 
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
bool	CMultiPlug::Connect( SERVICE_TYPE type, const std::string &ip, const int port )
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
			NETEVENT_CONNECT_TO( pClient, this, EVT_CONNECT, CMultiPlug, CMultiPlug::OnConnect );
			NETEVENT_CONNECT_TO( pClient, this, EVT_DISCONNECT, CMultiPlug, CMultiPlug::OnDisconnect );
			AddChild(pClient);

			//BOOST_FOREACH(auto &protocol, GetProtocolListeners())
			//{
			//	pClient->AddProtocolListener(protocol);
			//}
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
void	CMultiPlug::SetSessionFactory( ISessionFactory *ptr )
{
	SAFE_DELETE(m_pSessionFactory);
	m_pSessionFactory = ptr;
	if (m_pServer)
		m_pServer->SetSessionFactory(m_pSessionFactory->Clone());
}


/**
 @brief SetGroupFactory
 */
void	CMultiPlug::SetGroupFactory( IGroupFactory *ptr )
{
	SAFE_DELETE(m_pGroupFactory);
	m_pGroupFactory = ptr;
	if (m_pServer && ptr)
		m_pServer->SetGroupFactory( ptr->Clone() );
}


/**
 @brief OnConnect
 */
void	CMultiPlug::OnConnect( CNetEvent &event )
{
	m_State = RUN;
	SearchEventTable(CNetEvent(EVT_CONNECT, this));// Event Propagate
}


/**
 @brief OnDisconnect
 */
void	CMultiPlug::OnDisconnect( CNetEvent &event )
{
	if (m_ServiceType == SERVER)
		m_State = END;
	SearchEventTable(CNetEvent(EVT_DISCONNECT, this)); // Event Propagate

	// disconnect �� client ����
	if (CLIENT == m_ServiceType)
	{
		auto it = m_Clients.find(event.GetHandler()->GetNetId());
		if (m_Clients.end() == it)
			return;

		it->second->Stop();
		RemoveChild(it->second->GetNetId());

		m_Clients.remove( event.GetHandler()->GetNetId() );
		m_Clients.apply_removes();

		// ���� clients �� �Ű�����, ���߿� ��������.
		m_RemoveClients.insert( CoreClients_::value_type(event.GetHandler()->GetNetId(), 
			(CCoreClient*)event.GetHandler().Get() ) );
	}
}


/**
 @brief AddProtocolListener
 */
//bool	CMultiPlug::AddProtocolListener(ProtocolListenerPtr pListener)
//{
//	CPlug::AddProtocolListener(pListener);
//	if (m_pServer)
//		m_pServer->AddProtocolListener(pListener);
//	BOOST_FOREACH(auto &client, m_Clients.m_Seq)
//	{
//		client->AddProtocolListener(pListener);
//	}
//	return true;
//}


/**
 @brief RemoveProtocolListener
 */
//bool	CMultiPlug::RemoveProtocolListener(ProtocolListenerPtr pListener)
//{
//	CPlug::RemoveProtocolListener(pListener);
//	if (m_pServer)
//		m_pServer->RemoveProtocolListener(pListener);
//	BOOST_FOREACH(auto &client, m_Clients.m_Seq)
//	{
//		client->RemoveProtocolListener(pListener);
//	}
//	return true;
//}


/**
 @brief return CoreClient 
 */
CoreClientPtr CMultiPlug::GetClient(netid netId)
{
	auto it = m_Clients.find(netId);
	if (m_Clients.end() == it)
		return NULL;
	return it->second;
}


/**
 @brief return CoreClient from correspond server netid
 */
CoreClientPtr CMultiPlug::GetClientFromServerNetId(netid serverNetId)
{
	BOOST_FOREACH(auto &client, m_Clients.m_Seq)
	{
		if (client && client->GetServerNetId() == serverNetId)
			return client;
	}
	return NULL;
}


/**
 @brief ���� �ڽ�, Ŭ���̾�Ʈ �ڽ�, Ȥ�� ����Ʈ Ŭ���̾�Ʈ.
 */
SessionPtr CMultiPlug::GetSession(netid netId)
{
	if (GetServer())
	{
		if (GetServer()->GetNetId() == netId)
			return GetServer();
		if (SessionPtr ptr = GetServer()->GetSession(netId))
			return ptr;
	}
	else
	{
		if (CoreClientPtr ptr = GetClient(netId))
			return ptr;
		return GetClientFromServerNetId(netId);
	}
	return NULL;
}