
#include "stdafx.h"
#include "SubServerConnector.h"


using namespace network;


CSubServerConnector::CSubServerConnector()
{

}

CSubServerConnector::~CSubServerConnector()
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
void	CSubServerConnector::OnConnectNetGroupController()
{
	multinetwork::CNetGroupDelegation::OnConnectNetGroupController();
	AddProtocolListener( this );
	GetConnector()->SetSessionFactory( new CRemoteServerFactory() );
}


/**
 @brief ���� ������κ��� ���� ���� ������ ������Ʈ �޴´�.
 */
bool CSubServerConnector::SendServerInfo(IProtocolDispatcher &dispatcher, netid senderId, 
	const std::string &svrType, const int &userCount)
{
	//CRemoteServer *pRemoteSvr = dynamic_cast<CRemoteServer*>(
	//	CheckClientNetId(GetServer(), senderId, NULL, &dispatcher));
	//RETV(!pRemoteSvr, false);
	//pRemoteSvr->SetUserCount(userCount);

	auto it = m_RemoteServers.find(senderId);
	if (m_RemoteServers.end() == it)
	{
		CRemoteServer *pNewSvr = new CRemoteServer();
		pNewSvr->SetNetId(senderId);
		pNewSvr->SetUserCount(userCount);
		m_RemoteServers.insert( RemoteServers::value_type(senderId, pNewSvr) );
	}
	else
	{
		it->second->SetUserCount(userCount);
	}

	return true;
}


/**
 @brief GetSubServerInfo
 */
std::list<SSubServerInfo> CSubServerConnector::GetSubServerInfo()
{
	std::list<SSubServerInfo> servers;
	NetGroupControllerPtr pCtrl = GetConnector();
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

	return  servers;
}
