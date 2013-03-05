//------------------------------------------------------------------------
// Name:    NetLauncher.h
// Author:  jjuiddong
// Date:    12/24/2012
// 
// 
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class CServer;
	class CClient;
	class CCoreClient;
	class CNetLauncher : public common::CSingleton<CNetLauncher>
	{
	public:
		CNetLauncher() {}
		virtual ~CNetLauncher() {}

	public:
		bool	LaunchServer(ServerPtr pSvr, int port);
		bool	LaunchClient(ClientPtr pClient, const std::string &ip, int port);
		bool	LaunchCoreClient(CoreClientPtr pClient, const std::string &ip, int port);
	};
}
