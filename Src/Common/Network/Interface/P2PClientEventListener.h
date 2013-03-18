//------------------------------------------------------------------------
// Name:    P2PClientEventListener.h
// Author:  jjuiddong
// Date:    3/5/2013
// 
// P2PClient에서 발생하는 이벤트를 받을 수 있는 인터페이스 다.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class IP2PClientEventListener
	{
	public:
		virtual ~IP2PClientEventListener() {}
		virtual void		OnP2PCreate(P2PClientPtr client) {}
		virtual void		OnP2PDisconnect(P2PClientPtr client) {}
		virtual void		OnMemberJoin(P2PClientPtr client, netid clientId) {}
		virtual void		OnMemberLeave(P2PClientPtr client, netid clientId) {}
	};

}
