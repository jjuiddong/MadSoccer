
#include "stdafx.h"
#include "GameServer.h"
#include "GamePlayer.h"

using namespace network;

CGameServer::CGameServer()
{

}

CGameServer::~CGameServer()
{

}


/**
 @brief ReqMovePlayer
 */
bool CGameServer::ReqMovePlayer(server_network::ReqMovePlayer_Packet &packet)
{
	CSession *pClient = CheckClientId(GetServer(), packet.id, 0, NULL, NULL);
	if (pClient) // Already exist
	{///!!Error
		clog::Error( clog::ERROR_PROBLEM, 0, "ReqMovePlayer Player already exist netid: %d, id=%s", 
			pClient->GetNetId(), pClient->GetName().c_str() );
		m_SvrNetworkProtocol.AckMovePlayer(packet.senderId, SEND_T, error::ERR_MOVEUSER_ALREADY_EXIST,
			packet.id, packet.groupId, packet.ip, packet.port);
		return false;
	}

	GroupPtr pGroup = GetServer()->GetRootGroup().GetChild( packet.groupId );
	if (!pGroup)
	{
		clog::Error( clog::ERROR_PROBLEM, 0, "ReqMovePlayer not found group groupId: %d ", packet.groupId );
		m_SvrNetworkProtocol.AckMovePlayer(packet.senderId, SEND_T, error::ERR_NOT_FOUND_GROUP,
			packet.id, packet.groupId, packet.ip, packet.port);
		return false;
	}

	/// Add Player
	CGamePlayer *pPlayer = new CGamePlayer();
	pPlayer->SetName(packet.id);
	pPlayer->SetCertifyKey(packet.c_key);
	GetServer()->AddPlayer( pPlayer );

	if (!pGroup->AddPlayer( pGroup->GetId(), pPlayer->GetNetId() ))
	{
		clog::Error( clog::ERROR_PROBLEM, 0, "ReqMovePlayer player group join Error groupId: %d, playerId=%d", 
			packet.groupId, pPlayer->GetNetId() );
		m_SvrNetworkProtocol.AckMovePlayer(packet.senderId, SEND_T, error::ERR_NOT_JOIN_GROUP,
			packet.id, packet.groupId, packet.ip, packet.port);

		GetServer()->RemovePlayer( pPlayer->GetNetId() );
		return false;
	}

	m_SvrNetworkProtocol.AckMovePlayer(packet.senderId, SEND_T, error::ERR_SUCCESS, packet.id, packet.groupId, packet.ip, packet.port);
	return true;
}


/**
 @brief ReqMoveplayerCancel
 */
bool CGameServer::ReqMovePlayerCancel(server_network::ReqMovePlayerCancel_Packet &packet)
{

	return true;
}


/**
 @brief ReqCreateGroup
 */
bool CGameServer::ReqCreateGroup(server_network::ReqCreateGroup_Packet &packet)
{

	return true;
}


/**
 @brief SendServerInfo
 */
bool CGameServer::SendServerInfo(server_network::SendServerInfo_Packet &packet)
{

	return true;
}

