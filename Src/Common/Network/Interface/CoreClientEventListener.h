//------------------------------------------------------------------------
// Name:    CoreClientEventListener.h
// Author:  jjuiddong
// Date:    3/5/2013
// 
// CoreClient가 발생시키는 이벤트를 받을 수 있는 인터페이스다.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class ICoreClientEventListener
	{
	public:
		virtual void		OnCoreClientConnect(CoreClientPtr client) {}
		virtual void		OnClientDisconnect(CoreClientPtr client) {}
		//virtual void		OnMemberJoin(CoreClientPtr client) {}
		//virtual void		OnMemberLeave(CoreClientPtr client) {}
	};
}
