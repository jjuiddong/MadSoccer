
#ifndef __NETWORK_H__
#define __NETWORK_H__


#include "NetworkDef.h"
#include "Service/Server.h"
#include "Service/Client.h"


namespace network
{
	class CServer;
	class CClient;

	// Server
	bool		StartServer(int port, CServer *pSvr);
	bool		StopServer(CServer *pSvr);
	CServer*	GetServer(int serverId);

	// Client
	bool		StartClient(const std::string &ip, int port, CClient *pClt);
	bool		StopClient(CClient *pClt);
	CClient*	GetClient(int clientId);

	// Common
	bool		Init(int logicThreadCount);
	void		Proc();
	void		Clear();

	// Debug
	std::string ToString();

};

#endif // __NETWORK_H__
