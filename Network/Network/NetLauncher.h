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
	class CNetLauncher : public common::CSingleton<CNetLauncher>
	{
	public:
		CNetLauncher() {}
		virtual ~CNetLauncher() {}

	public:
		bool	LaunchServer(CServer *pSvr, int port);
		bool	LaunchClient(CClient *pClient, const std::string &ip, int port);
	};
}
