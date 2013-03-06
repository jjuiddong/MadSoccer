//------------------------------------------------------------------------
// Name:    ClientEventListener.h
// Author:  jjuiddong
// Date:    3/5/2013
// 
// CClient에서 발생되는 이벤트를 받을 수 있는 리스너 인터페이스 다.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class IClientEventListener
	{
	public:
		// Server / Client Communication
		virtual void		OnClientConnect(ClientPtr client) {}
		virtual void		OnClientDisconnect(ClientPtr client) {}

		// P2P Communication
		virtual void		OnP2PConnect(ClientPtr client) {}
		virtual void		OnP2PDisconnect(ClientPtr client) {}

		virtual void		OnMemberJoin(ClientPtr client) {}
		virtual void		OnMemberLeave(ClientPtr client) {}
	};
}
