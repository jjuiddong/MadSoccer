//------------------------------------------------------------------------
// Name:    ServerEventListener.h
// Author:  jjuiddong
// Date:    3/5/2013
// 
// Server���� �߻���Ű�� �̺�Ʈ�� �޴� ������ Ŭ������.
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
