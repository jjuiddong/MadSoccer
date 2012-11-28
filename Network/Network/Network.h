
#ifndef __NETWORK_H__
#define __NETWORK_H__


#include "NetworkDef.h"
#include "Server.h"
#include "Client.h"


namespace network
{
	class CServer;
	class CClient;

	// Server
	bool StartServer(int port, CServer *pSvr);
	bool StopServer(CServer *pSvr);

	// Client
	bool StartClient(std::string ip, int port, CClient *pClt);
	bool StopClient(CClient *pClt);

	// Common
	void Proc();

};

#endif // __NETWORK_H__
