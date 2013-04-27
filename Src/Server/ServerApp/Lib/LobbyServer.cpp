
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
// ���� �߰�
//------------------------------------------------------------------------
bool CLobbyServer::AddUser(CUser *pUser)
{
	UserItor it = m_Users.find(pUser->GetNetId());
	if (m_Users.end() != it)
		return false; // �̹� �����Ѵٸ� ����
	m_Users.insert( UserMap::value_type(pUser->GetNetId(), pUser) );
	return true;
}


//------------------------------------------------------------------------
// ���� ����
// ���ڷ� �Ѿ�� pUser �޸𸮸� �����Ѵ�.
//------------------------------------------------------------------------
bool CLobbyServer::RemoveUser(CUser *pUser)
{
	UserItor it = m_Users.find(pUser->GetNetId());
	if (m_Users.end() == it)
		return false; // ���ٸ� ����
	delete pUser;
	m_Users.erase(it);
	return true;
}


//------------------------------------------------------------------------
// ���� ����
// �޸𸮱��� ���ŵȴ�.
//------------------------------------------------------------------------
bool CLobbyServer::RemoveUser(netid netId)
{
	UserItor it = m_Users.find(netId);
	if (m_Users.end() == it)
		return false; // ���ٸ� ����
	delete it->second;
	m_Users.erase(it);
	return true;
}


//------------------------------------------------------------------------
// ���� ���
//------------------------------------------------------------------------
UserPtr	CLobbyServer::GetUser(netid netId)
{
	UserItor it = m_Users.find(netId);
	if (m_Users.end() == it)
		return NULL; // ���ٸ� ����
	return it->second;
}


//------------------------------------------------------------------------
// �� �߰�
//------------------------------------------------------------------------
bool CLobbyServer::AddRoom(CRoom *pRoom)
{
	RoomItor it = m_Rooms.find(pRoom->GetId());
	if (m_Rooms.end() != it)
		return false; // �̹� �����Ѵٸ� ����
	m_Rooms.insert( RoomMap::value_type(pRoom->GetId(), pRoom) );
	return true;
}


//------------------------------------------------------------------------
// ������
// �޸𸮱��� ���ŵȴ�.
//------------------------------------------------------------------------
bool CLobbyServer::RemoveRoom(CRoom *pRoom)
{
	RoomItor it = m_Rooms.find(pRoom->GetId());
	if (m_Rooms.end() == it)
		return false; // ���ٸ� ����
	delete it->second;
	m_Rooms.erase(it);
	return true;
}


//------------------------------------------------------------------------
// ������
// �޸𸮱��� ���ŵȴ�.
//------------------------------------------------------------------------
bool CLobbyServer::RemoveRoom(int roomId)
{
	RoomItor it = m_Rooms.find(roomId);
	if (m_Rooms.end() == it)
		return false; // ���ٸ� ����
	delete it->second;
	m_Rooms.erase(it);
	return true;
}


//------------------------------------------------------------------------
// ������ ���
//------------------------------------------------------------------------
RoomPtr CLobbyServer::GetRoom(int roomId)
{
	RoomItor it = m_Rooms.find(roomId);
	if (m_Rooms.end() == it)
		return NULL; // ���ٸ� ����
	return it->second;
}


//------------------------------------------------------------------------
// �������� userId Ŭ���̾�Ʈ���� ������.
//------------------------------------------------------------------------
void CLobbyServer::SendRooms(netid userId)
{
	BOOST_FOREACH(RoomMap::value_type &vk, m_Rooms)
	{

	}
}


//------------------------------------------------------------------------
// ���������� userId Ŭ���̾�Ʈ���� ������.
//------------------------------------------------------------------------
void CLobbyServer::SendUsers(netid userId)
{

}


//------------------------------------------------------------------------
// Ŭ���̾�Ʈ�� ������ �پ��� �� ȣ��ȴ�.
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
// Ŭ���̾�Ʈ�� ������ ���� ������ ������ �� ȣ��ȴ�.
//------------------------------------------------------------------------
void CLobbyServer::OnClientLeave(CNetEvent &event)
{
	if (!RemoveUser(event.GetNetId()))
	{
		clog::Error(clog::ERROR_PROBLEM,  "RemoveUser() Faile!! id = %d\n", event.GetNetId());
	}
}


//------------------------------------------------------------------------
// ��Ʈ������ ��ȯ, �ַ� ����뿡 ���õ� ������ ��Ʈ������ ������.
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
		// �ֱ������� ���� ������ Login�������� ������.
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
