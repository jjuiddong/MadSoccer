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

	CRemoteSubServer();
	void		SetP2PLink( const std::vector<std::string> &v );	
	void		SetInputLink( const std::vector<std::string> &v );
	void		SetOutputLink( const std::vector<std::string> &v );

	void		GetLinkInfo( const std::string &linkSvrType, OUT std::vector<network::SHostInfo> &v );

	int		GetToBindInnerPort(int basePort);
	int		GetToBindOuterPort(int basePort);

	void		SetInnerBindPort( const std::string &linkSvrType, int bindPort );
	void		SetOuterBindPort( const std::string &linkSvrType, int bindPort );

protected:
	void		GetLinkInfo( const Links &linkServers,  const std::string &linkSvrType, OUT std::vector<network::SHostInfo> &v );
	bool		IsBindPort( const Links &linkServers, int port );
	SLinkState* GetLinkState( network::SERVICE_TYPE type, const std::string &linkSvrType );
	SLinkState* GetLinkState( Links &links, network::SERVICE_TYPE type, const std::string &linkSvrType );

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
