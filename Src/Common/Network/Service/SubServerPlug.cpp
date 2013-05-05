
#include "stdafx.h"
#include "SubServerPlug.h"

using namespace network;


CSubServerPlug::CSubServerPlug(const std::string &appSvrType) :
	m_AppSvrType(appSvrType)
{

}

CSubServerPlug::~CSubServerPlug()
{
	BOOST_FOREACH(auto &svr, m_RemoteServers.m_Seq)
	{
		SAFE_DELETE(svr);
	}
	m_RemoteServers.clear();
}


/**
 @brief OnConnectNetGroupController
 */
void	CSubServerPlug::OnConnectMultiPlug()
{
	multinetwork::CMultiPlugDelegation::OnConnectMultiPlug();

	AddProtocolListener( this );
	GetMultiPlug()->SetSessionFactory( new CRemoteServerFactory() );
	RegisterProtocol( &m_ServerNetwork_Protocol );

	NETEVENT_CONNECT(EVT_CONNECT, CSubServerPlug, CSubServerPlug::OnConnectSubLink);
	NETEVENT_CONNECT(EVT_CLIENT_JOIN, CSubServerPlug, CSubServerPlug::OnConnectSubLink);
}


/**
 @brief  Call, client connection, or server attach remote client
 */
void	CSubServerPlug::OnConnectSubLink(CNetEvent &event )
{
	m_ServerNetwork_Protocol.SendServerInfo( event.GetNetId(), SEND_T, 
		m_AppSvrType, "localhost", 0, 0 );
}


/**
 @brief ���� ������κ��� ���� ���� ������ ������Ʈ �޴´�.
 */
bool CSubServerPlug::SendServerInfo(IProtocolDispatcher &dispatcher, netid senderId, 
	const std::string &svrType, const std::string &ip, const int &port, const int &userCount)
{
	auto it = m_RemoteServers.find(senderId);
	if (m_RemoteServers.end() == it)
	{
		CRemoteServer *pNewSvr = new CRemoteServer();
		pNewSvr->SetNetId(senderId);
		pNewSvr->SetIp( ip );
		pNewSvr->SetPort(port);
		pNewSvr->SetNetId(senderId);
		pNewSvr->SetUserCount(userCount);
		m_RemoteServers.insert( RemoteServers::value_type(senderId, pNewSvr) );
	}
	else
	{
		it->second->SetIp( ip );
		it->second->SetPort(port);
		it->second->SetUserCount(userCount);
	}

	return true;
}


/**
 @brief GetSubServerInfo
 */
std::list<SSubServerInfo> CSubServerPlug::GetSubServerInfo()
{
	std::list<SSubServerInfo> servers;
	MultiPlugPtr pCtrl = GetMultiPlug();
	RETV(!pCtrl, servers);

	if (SERVER == pCtrl->GetServiceType()) // server
	{
		RETV(!GetServer(), servers);
		BOOST_FOREACH(auto &svr, m_RemoteServers.m_Seq)
		{
			// ���ӵ� Ŭ���̾�Ʈ���� Ȯ���Ѵ�. ���ٸ� ����.
			CSession *pClient = GetServer()->GetSession( svr->GetNetId() );
			if (pClient)
			{
					SSubServerInfo info;
					info.serverId = svr->GetNetId();
					info.ip = svr->GetIp();
					info.portnum = svr->GetPort();
					info.userCnt = svr->GetUserCount();
					servers.push_back( info );
			}
			else
			{
				m_RemoteServers.remove( svr->GetNetId() );
			}
		}
	}
	else // client
	{
		RETV(!GetServer(), servers);
		BOOST_FOREACH(auto &svr, m_RemoteServers.m_Seq)
		{
			// ���ӵ� Ŭ���̾�Ʈ���� Ȯ���Ѵ�. ���ٸ� ����.
			CSession *pClient = GetClientFromServerNetId( svr->GetNetId() );
			if (pClient)
			{
				SSubServerInfo info;
				info.serverId = svr->GetNetId();
				info.ip = svr->GetIp();
				info.portnum = svr->GetPort();
				info.userCnt = svr->GetUserCount();
				servers.push_back( info );				
			}
			else
			{
				m_RemoteServers.remove( svr->GetNetId() );
			}
		}
	}

	m_RemoteServers.apply_removes();

	return  servers;
}
