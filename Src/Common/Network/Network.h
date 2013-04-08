
#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "NetworkDef.h"
#include "Service/Server.h"
#include "Service/Client.h"

namespace network
{
	using namespace marshalling;

	class CServerBasic;
	class CClient;

	// Server
	bool		StartServer(int port, ServerBasicPtr pSvr);
	bool		StopServer(ServerBasicPtr pSvr);
	ServerBasicPtr	GetServer(netid serverId);

	// Client
	bool		StartClient(const std::string &ip, int port, ClientBasicPtr pClt);
	bool		StopClient(ClientBasicPtr pClt);
	ClientBasicPtr	GetClient(netid clientId);

	// Multi Network
	bool		StartMultiNetwork();
	bool		ConnectDelegation( const std::string &linkSvrType, NetGroupDelegationPtr ptr);

	// Common
	bool		Init(int logicThreadCount, const std::string &svrConfigFileName="");
	void		Proc();
	void		Clear();

	// Debug
	std::string	ToString();

};

#endif // __NETWORK_H__
