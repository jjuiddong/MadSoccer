
#include "stdafx.h"
#include "LobbyServer.h"
#include "../DataStructure/UserLobby.h"


using namespace network;

CLobbyServer::CLobbyServer() :
	m_pBasicPrtHandler(NULL)
{

}

CLobbyServer::~CLobbyServer() 
{
	RemoveProtocolListener(m_pBasicPrtHandler);
	SAFE_DELETE(m_pBasicPrtHandler);

}


/**
 @brief OnConnectNetGroupController
 */
void	CLobbyServer::OnConnectMultiPlug()
{
	CServer::OnConnectMultiPlug();

	MultiPlugPtr pLoginMultiPlug = multinetwork::CMultiNetwork::Get()->GetMultiPlug("loginsvr");
	if (!pLoginMultiPlug)
	{
		clog::Error( clog::ERROR_CRITICAL, "CLobbyServer Init Error!! not found lobbysvr netgroupcontroller" );
		return;
	}
	MultiPlugPtr pCertifyMultiPlug = multinetwork::CMultiNetwork::Get()->GetMultiPlug("certifysvr");
	if (!pCertifyMultiPlug)
	{
		clog::Error( clog::ERROR_CRITICAL, "CLobbyServer Init Error!! not found certify netgroupcontroller" );
		return;
	}

	AddChild( pLoginMultiPlug );
	AddChild( pCertifyMultiPlug );

	GetServer()->SetOption( true );
	
	m_pBasicPrtHandler = new CBasicC2SHandler_LobbySvr(*pCertifyMultiPlug, *GetServer());

	pLoginMultiPlug->RegisterProtocol( &m_SvrNetworkProtocol );
	pCertifyMultiPlug->RegisterProtocol(&m_CertifyProtocol);

	RegisterProtocol(&m_LoginProtocol);
	RegisterProtocol(&m_BasicProtocol);
	AddProtocolListener( this );
	AddProtocolListener(m_pBasicPrtHandler);

	NETEVENT_CONNECT( EVT_CLIENT_JOIN, CLobbyServer, CLobbyServer::OnClientJoin );
	NETEVENT_CONNECT( EVT_CLIENT_LEAVE, CLobbyServer, CLobbyServer::OnClientLeave );
	EVENT_CONNECT_TO( GetServer(), this, EVT_TIMER, CLobbyServer, CLobbyServer::OnTimer );

	GetServer()->AddTimer(ID_TIMER_REFRESH, REFRESH_TIMER_INTERVAL);
}


//------------------------------------------------------------------------
// 유저 추가
//------------------------------------------------------------------------
bool CLobbyServer::AddUser(CUser *pUser)
{
	auto it = m_Users.find(pUser->GetNetId());
	if (m_Users.end() != it)
		return false; // 이미 존재한다면 실패
	m_Users.insert( Users_::value_type(pUser->GetNetId(), pUser) );
	return true;
}


//------------------------------------------------------------------------
// 유저 제거
// 인자로 넘어온 pUser 메모리를 제거한다.
//------------------------------------------------------------------------
bool CLobbyServer::RemoveUser(CUser *pUser)
{
	auto it = m_Users.find(pUser->GetNetId());
	if (m_Users.end() == it)
		return false; // 없다면 실패
	m_Users.remove(pUser->GetNetId());
	m_Users.apply_removes();
	delete pUser;
	return true;
}


//------------------------------------------------------------------------
// 유저 제거
// 메모리까지 제거된다.
//------------------------------------------------------------------------
bool CLobbyServer::RemoveUser(netid netId)
{
	auto it = m_Users.find(netId);
	if (m_Users.end() == it)
		return false; // 없다면 실패
	delete it->second;
	m_Users.remove(netId);
	m_Users.apply_removes();
	return true;
}


//------------------------------------------------------------------------
// 유저 얻기
//------------------------------------------------------------------------
UserPtr	CLobbyServer::GetUser(netid netId)
{
	auto it = m_Users.find(netId);
	if (m_Users.end() == it)
		return NULL; // 없다면 실패
	return it->second;
}


////------------------------------------------------------------------------
//// 방 추가
////------------------------------------------------------------------------
//bool CLobbyServer::AddRoom(CRoom *pRoom)
//{
//	RoomItor it = m_Rooms.find(pRoom->GetId());
//	if (m_Rooms.end() != it)
//		return false; // 이미 존재한다면 실패
//	m_Rooms.insert( RoomMap::value_type(pRoom->GetId(), pRoom) );
//	return true;
//}
//
//
////------------------------------------------------------------------------
//// 방제거
//// 메모리까지 제거된다.
////------------------------------------------------------------------------
//bool CLobbyServer::RemoveRoom(CRoom *pRoom)
//{
//	RoomItor it = m_Rooms.find(pRoom->GetId());
//	if (m_Rooms.end() == it)
//		return false; // 없다면 실패
//	delete it->second;
//	m_Rooms.erase(it);
//	return true;
//}
//
//
////------------------------------------------------------------------------
//// 방제거
//// 메모리까지 제거된다.
////------------------------------------------------------------------------
//bool CLobbyServer::RemoveRoom(int roomId)
//{
//	RoomItor it = m_Rooms.find(roomId);
//	if (m_Rooms.end() == it)
//		return false; // 없다면 실패
//	delete it->second;
//	m_Rooms.erase(it);
//	return true;
//}
//
//
////------------------------------------------------------------------------
//// 방정보 얻기
////------------------------------------------------------------------------
//RoomPtr CLobbyServer::GetRoom(int roomId)
//{
//	RoomItor it = m_Rooms.find(roomId);
//	if (m_Rooms.end() == it)
//		return NULL; // 없다면 실패
//	return it->second;
//}


////------------------------------------------------------------------------
//// 방정보를 userId 클라이언트에게 보낸다.
////------------------------------------------------------------------------
//void CLobbyServer::SendRooms(netid userId)
//{
//	BOOST_FOREACH(RoomMap::value_type &vk, m_Rooms)
//	{
//
//	}
//}


//------------------------------------------------------------------------
// 유저정보를 userId 클라이언트에게 보낸다.
//------------------------------------------------------------------------
void CLobbyServer::SendUsers(netid userId)
{

}


//------------------------------------------------------------------------
// 클라이언트가 서버에 붙었을 때 호출된다.
//------------------------------------------------------------------------
void CLobbyServer::OnClientJoin(CNetEvent &event)
{
	CUserLobby *pUser = new CUserLobby();
	pUser->SetNetId(event.GetNetId());
	if (!AddUser( pUser ))
	{
		clog::Error( clog::ERROR_PROBLEM, "AddUser() Fail!! id = %d\n", event.GetNetId());
		delete pUser;
	}
}


//------------------------------------------------------------------------
// 클라이언트가 서버로 부터 떨어져 나갔을 때 호출된다.
//------------------------------------------------------------------------
void CLobbyServer::OnClientLeave(CNetEvent &event)
{
	if (!RemoveUser(event.GetNetId()))
	{
		clog::Error(clog::ERROR_PROBLEM,  "RemoveUser() Fail!! id = %d\n", event.GetNetId());
	}
}


//------------------------------------------------------------------------
// 스트링으로 변환, 주로 디버깅에 관련된 정보를 스트링으로 보낸다.
//------------------------------------------------------------------------
std::string CLobbyServer::ToString()
{
	common::AutoCSLock cs(GetServer()->GetCS());

	std::stringstream ss;
	ss << "Session : " << GetServer()->GetSessions().size() << std::endl;
	BOOST_FOREACH(auto &client, GetServer()->GetSessions())
	{
		ss << "netid: " << client->GetNetId() << ", sock: " << client->GetSocket() << std::endl;
	}
	return ss.str();
}


/**
 @brief 
 */
void	CLobbyServer::OnTimer( CEvent &event )
{
	if (ID_TIMER_REFRESH == event.GetParam())
	{
		// 주기적으로 서버 정보를 Login서버에게 보낸다.
		if (GetServer() && GetServer()->IsServerOn())
		{
			MultiPlugPtr pLoginSvrController = multinetwork::CMultiNetwork::Get()->GetMultiPlug("loginsvr");
			if (pLoginSvrController)
			{
				pLoginSvrController->RegisterProtocol( &m_SvrNetworkProtocol );
				m_SvrNetworkProtocol.SendServerInfo( ALL_NETID, network::SEND_T, "lobbysvr", 
					GetServer()->GetIp(), GetServer()->GetPort(), GetServer()->GetSessions().size() );
			}
		}
	}
}


//------------------------------------------------------------------------
//  Test Packet
//------------------------------------------------------------------------
//void CLobbyServer::ReqLogIn(netid senderId, const std::string &id, const std::string &password)
//{
//	m_LoginProtocol.AckLogIn(senderId, SEND_TARGET, error::ERR_SUCCESS, id, senderId);
//}


/**
 @brief ReqMoveUser
 */
bool CLobbyServer::ReqMoveUser(IProtocolDispatcher &dispatcher, netid senderId, 
	const std::string &id, const netid &userId, const std::string &ip, const int &port)
{
	CSession *pClient = CheckClientId(GetServer(), id, userId, NULL, NULL);
	if (pClient) // Already exist
	{ /// !!Error
		clog::Error( clog::ERROR_PROBLEM, 0, "CLobbyServer::ReqMoveUser user already exist netid: %d, id=%s", 
			pClient->GetNetId(), pClient->GetName().c_str() );
		m_SvrNetworkProtocol.AckMoveUser(senderId, SEND_T, error::ERR_MOVEUSER_ALREADY_EXIST,
			id, userId, ip, port);
		return false;
	}

	//multinetwork::CMultiNetwork::Get()->RegisterProtocol(&m_SvrNetworkProtocol) ;
	m_SvrNetworkProtocol.AckMoveUser(senderId, SEND_T, error::ERR_SUCCESS, id, userId, ip, port);
	
	return true;
}
