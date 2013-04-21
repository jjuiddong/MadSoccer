/**
Name:   LoginServer.h
Author:  jjuiddong
Date:    4/20/2013


*/
#pragma once

DECLARE_TYPE_NAME(CLoginServer)
class CLoginServer : public network::multinetwork::CNetGroupDelegation
	, public network::AllProtocolDisplayer
	, public memmonitor::Monitor<CLoginServer, TYPE_NAME(CLoginServer)>
{
public:
	CLoginServer();
	virtual ~CLoginServer();


};
