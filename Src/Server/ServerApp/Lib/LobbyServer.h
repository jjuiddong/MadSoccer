/**
Name:   LobbyServer.h
Author:  jjuiddong
Date:    12/29/2012

로비 서버
*/
#pragma once

#include "NetProtocol/Src/login_Protocol.h"
#include "NetProtocol/Src/login_ProtocolListener.h"
#include "NetProtocol/Src/basic_Protocol.h"
#include "NetProtocol/Src/basic_ProtocolListener.h"

DECLARE_TYPE_NAME(CLobbyServer)
class CLobbyServer	: public network::CServer
	//: public network::multinetwork::CNetGroupDelegation
								, public login::c2s_ProtocolListener
								, public network::AllProtocolDisplayer
								, public memmonitor::Monitor<CLobbyServer, TYPE_NAME(CLobbyServer)>
{
public:
	CLobbyServer();
	virtual ~CLobbyServer();

	bool			AddUser(network::CUser *pUser);
	bool			RemoveUser(network::CUser *pUser);
	bool			RemoveUser(netid netId);
	UserPtr		GetUser(netid netId);
	
	bool			AddRoom(network::CRoom *pRoom);
	bool			RemoveRoom(network::CRoom *pRoom);
	bool			RemoveRoom(int roomId);
	RoomPtr	GetRoom(int roomId);

	// 패킷 보내기
	void			SendRooms(network::CUser *pUser);
	void			SendUsers(network::CUser *pUser);

	login::s2c_Protocol& GetLogInProtocol() { return m_LoginProtocol; }

	// Debug용
	std::string		ToString();

protected:
	void			SendRooms(netid userId);
	void			SendUsers(netid userId);

	// Event Handler
	virtual void	OnConnectNetGroupController() override;

	// Network Event Handler
	void			OnClientJoin(network::CNetEvent &event);
	void			OnClientLeave(network::CNetEvent &event);
	void			OnClientLeave2(network::CEvent &event) {}


	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Test
	//virtual void ReqLogIn(netid senderId, const std::string &id, const std::string &password) override;

private:
	UserMap				m_Users;
	RoomMap			m_Rooms;

	// Protocol
	login::s2c_Protocol	m_LoginProtocol;
	basic::s2c_Protocol	m_BasicProtocol;

};
