
#include "stdafx.h"
#include "LobbyServer.h"
#include "../DataStructure/UserLobby.h"

#include "NetProtocol/Src/server_network_Protocol.cpp"
#include "NetProtocol/Src/server_network_ProtocolListener.cpp"


using namespace network;

CLobbyServer::CLobbyServer()
{
}

CLobbyServer::~CLobbyServer() 
{

}


/**
 @brief OnConnectNetGroupController
 */
void	CLobbyServer::OnConnectNetGroupController()
{
	CServer::OnConnectNetGroupController();

	NetGroupControllerPtr pLoginSvrController = multinetwork::CMultiNetwork::Get()->GetController("loginsvr");
	if (!pLoginSvrController)
	{
		clog::Error( clog::ERROR_CRITICAL, "CLobbyServer Init Error!! not found lobbysvr netgroupcontroller" );
		return;
	}
	NetGroupControllerPtr pCertifySvrController = multinetwork::CMultiNetwork::Get()->GetController("certifysvr");
	if (!pCertifySvrController)
	{
		clog::Error( clog::ERROR_CRITICAL, "CLobbyServer Init Error!! not found certify netgroupcontroller" );
		return;
	}

	pLoginSvrController->RegisterProtocol(&m_SvrNetworkProtocol);
	pCertifySvrController->RegisterProtocol(&m_CertifyProtocol);

	RegisterProtocol(&m_LoginProtocol);
	RegisterProtocol(&m_BasicProtocol);
	AddProtocolListener( this );

	NETEVENT_CONNECT_TO(pLoginSvrController, this, EVT_CONNECT, CLobbyServer, CLobbyServer::OnSubServerConnect);
	NETEVENT_CONNECT_TO(pCertifySvrController, this, EVT_CONNECT, CLobbyServer, CLobbyServer::OnSubServerConnect);

	NETEVENT_CONNECT( EVT_CLIENT_JOIN, CLobbyServer, CLobbyServer::OnClientJoin );
	NETEVENT_CONNECT( EVT_CLIENT_LEAVE, CLobbyServer, CLobbyServer::OnClientLeave );
	EVENT_CONNECT_TO( GetServer(), this, EVT_TIMER, CLobbyServer, CLobbyServer::OnTimer );

	GetServer()->AddTimer(ID_TIMER_REFRESH, REFRESH_TIMER_INTERVAL);
}


/**
 @brief OnSubServerConnect
 */
void	CLobbyServer::OnSubServerConnect(network::CNetEvent &event)
{

}


//------------------------------------------------------------------------
// 유저 추가
//------------------------------------------------------------------------
bool CLobbyServer::AddUser(CUser *pUser)
{
	UserItor it = m_Users.find(pUser->GetNetId());
	if (m_Users.end() != it)
		return false; // 이미 존재한다면 실패
	m_Users.insert( UserMap::value_type(pUser->GetNetId(), pUser) );
	return true;
}


//------------------------------------------------------------------------
// 유저 제거
// 인자로 넘어온 pUser 메모리를 제거한다.
//------------------------------------------------------------------------
bool CLobbyServer::RemoveUser(CUser *pUser)
{
	UserItor it = m_Users.find(pUser->GetNetId());
	if (m_Users.end() == it)
		return false; // 없다면 실패
	delete pUser;
	m_Users.erase(it);
	return true;
}


//------------------------------------------------------------------------
// 유저 제거
// 메모리까지 제거된다.
//------------------------------------------------------------------------
bool CLobbyServer::RemoveUser(netid netId)
{
	UserItor it = m_Users.find(netId);
	if (m_Users.end() == it)
		return false; // 없다면 실패
	delete it->second;
	m_Users.erase(it);
	return true;
}


//------------------------------------------------------------------------
// 유저 얻기
//------------------------------------------------------------------------
UserPtr	CLobbyServer::GetUser(netid netId)
{
	UserItor it = m_Users.find(netId);
	if (m_Users.end() == it)
		return NULL; // 없다면 실패
	return it->second;
}


//------------------------------------------------------------------------
// 방 추가
//------------------------------------------------------------------------
bool CLobbyServer::AddRoom(CRoom *pRoom)
{
	RoomItor it = m_Rooms.find(pRoom->GetId());
	if (m_Rooms.end() != it)
		return false; // 이미 존재한다면 실패
	m_Rooms.insert( RoomMap::value_type(pRoom->GetId(), pRoom) );
	return true;
}


//------------------------------------------------------------------------
// 방제거
// 메모리까지 제거된다.
//------------------------------------------------------------------------
bool CLobbyServer::RemoveRoom(CRoom *pRoom)
{
	RoomItor it = m_Rooms.find(pRoom->GetId());
	if (m_Rooms.end() == it)
		return false; // 없다면 실패
	delete it->second;
	m_Rooms.erase(it);
	return true;
}


//------------------------------------------------------------------------
// 방제거
// 메모리까지 제거된다.
//------------------------------------------------------------------------
bool CLobbyServer::RemoveRoom(int roomId)
{
	RoomItor it = m_Rooms.find(roomId);
	if (m_Rooms.end() == it)
		return false; // 없다면 실패
	delete it->second;
	m_Rooms.erase(it);
	return true;
}


//------------------------------------------------------------------------
// 방정보 얻기
//------------------------------------------------------------------------
RoomPtr CLobbyServer::GetRoom(int roomId)
{
	RoomItor it = m_Rooms.find(roomId);
	if (m_Rooms.end() == it)
		return NULL; // 없다면 실패
	return it->second;
}


//------------------------------------------------------------------------
// 방정보를 userId 클라이언트에게 보낸다.
//------------------------------------------------------------------------
void CLobbyServer::SendRooms(netid userId)
{
	BOOST_FOREACH(RoomMap::value_type &vk, m_Rooms)
	{

	}
}


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
		clog::Error( clog::ERROR_PROBLEM, "AddUser() Faile!! id = %d\n", event.GetNetId());
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
		clog::Error(clog::ERROR_PROBLEM,  "RemoveUser() Faile!! id = %d\n", event.GetNetId());
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
			m_SvrNetworkProtocol.SendServerInfo( SERVER_NETID, network::SEND_T, "lobbysvr", GetServer()->GetSessions().size() );
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
