
#include "stdafx.h"
#include "LobbyServer.h"
#include "../DataStructure/UserLobby.h"


using namespace network;

CLobbyServer::CLobbyServer() //: CServer(SERVICE_EXCLUSIVE_THREAD)
{
	RegisterProtocol(&m_LoginProtocol);
	RegisterProtocol(&m_BasicProtocol);
	AddProtocolListener( this );

	EVENT_CONNECT( EVT_CLIENT_JOIN, CLobbyServer, CLobbyServer::OnClientJoin );
	EVENT_CONNECT( EVT_CLIENT_LEAVE, CLobbyServer, CLobbyServer::OnClientLeave );
}

CLobbyServer::~CLobbyServer() 
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
	ss << "RemoteClient: " << GetServer()->GetRemoteClients().size() << std::endl;
	BOOST_FOREACH(RemoteClientMap::value_type &kv, GetServer()->GetRemoteClients())
	{
		ss << "netid: " << kv.second->GetId() << ", sock: " << kv.second->GetSocket() << std::endl;			
	}
	return ss.str();
}


//------------------------------------------------------------------------
//  Test Packet
//------------------------------------------------------------------------
void CLobbyServer::ReqLogIn(netid senderId, const std::string &id, const std::string &password)
{
	m_LoginProtocol.AckLogIn(senderId, SEND_TARGET, error::ERR_SUCCESS, id, senderId);
}
