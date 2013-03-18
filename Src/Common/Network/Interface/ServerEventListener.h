//------------------------------------------------------------------------
// Name:    ServerEventListener.h
// Author:  jjuiddong
// Date:    3/5/2013
// 
// Server에서 발생시키는 이벤트를 받는 리스너 클래스다.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class IServerEventListener
	{
	public:
		virtual ~IServerEventListener() {}
		virtual void		OnListen(ServerBasicPtr svr) {}
		virtual void		OnServerDisconnect(ServerBasicPtr svr) {}
		virtual void		OnClientJoin(ServerBasicPtr svr, netid netId) {}
		virtual void		OnClientLeave(ServerBasicPtr svr, netid netId) {}
	};
}
