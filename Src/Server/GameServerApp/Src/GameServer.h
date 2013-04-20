/**
Name:   GameServer.h
Author:  jjuiddong
Date:    4/18/2013

게임 서버 
*/
#pragma once

DECLARE_TYPE_NAME(CGameServer)
class CGameServer : public network::multinetwork::CNetGroupDelegation
								, public network::AllProtocolDisplayer
								, public memmonitor::Monitor<CGameServer, TYPE_NAME(CGameServer)>
{
public:
	CGameServer();
	virtual ~CGameServer();


};
