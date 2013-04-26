
#include "stdafx.h"
#include "SubServerConnector.h"


using namespace network;


CSubServerConnector::CSubServerConnector()
{

}

CSubServerConnector::~CSubServerConnector()
{

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
 @brief 서브 서버들로부터 현재 서버 정보를 업데이트 받는다.
 */
bool CSubServerConnector::SendServerInfo(IProtocolDispatcher &dispatcher, netid senderId, 
	const std::string &svrType, const int &userCount)
{
	CRemoteServer *pRemoteSvr = dynamic_cast<CRemoteServer*>(
		CheckClientNetId(GetServer(), senderId, NULL, &dispatcher));
	RETV(!pRemoteSvr, false);

	pRemoteSvr->SetUserCount(userCount);
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

	if (SERVER == pCtrl->GetServiceType())
	{
		RETV(!GetServer(), servers);

		BOOST_FOREACH(auto &client, GetServer()->GetSessions())
		{
			if (!client)
				continue;
			CRemoteServer *pSvr = dynamic_cast<CRemoteServer*>(client);
			if (!pSvr)
				continue;

			SSubServerInfo info;
			info.ip = pSvr->GetIp();
			info.portnum = 0;
			info.userCnt = pSvr->GetUserCount();
			servers.push_back( info );
		}
	}
	else
	{
		BOOST_FOREACH( auto &client, GetClients())
		{
			if (!client)
				continue;

			SSubServerInfo info;
			info.ip = client->GetServerIp();
			info.portnum = client->GetServerPort();
			//info.userCnt = pSvr->GetUserCount();
			servers.push_back( info );
		}
	}

	return  servers;
}
