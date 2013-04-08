/**
Name:   RemoteSubServer.h
Author:  jjuiddong
Date:    4/7/2013

서브 서버 정보를 표현한다.
*/
#pragma once

#include "LinkState.h"

/// Remote Sub Server class 
class CRemoteSubServer : public network::CRemoteClient
{
public:
	typedef std::vector<SLinkState> Links;

private:
	int m_OuterPort;
	Links m_InputLink;
	Links m_OutputLink;
	Links m_P2PLink;

};


/// CRemoteSubServer class Factory
class CRemoteSubServerFactory : public network::IRemoteClientFactory
{
	virtual network::CRemoteClient* New() { return new CRemoteSubServer(); }
	virtual network::IRemoteClientFactory* Clone() { return new CRemoteSubServerFactory(); }
};
