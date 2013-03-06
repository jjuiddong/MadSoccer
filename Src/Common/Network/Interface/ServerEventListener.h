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
		virtual void		OnListen(ServerPtr svr) {}
		virtual void		OnServerDisconnect(ServerPtr svr) {}
		virtual void		OnClientJoin(ServerPtr svr, netid netId) {}
		virtual void		OnClientLeave(ServerPtr svr, netid netId) {}
	};
}
