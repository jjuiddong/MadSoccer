//------------------------------------------------------------------------
// Name:    CoreClientEventListener.h
// Author:  jjuiddong
// Date:    3/5/2013
// 
// CoreClient�� �߻���Ű�� �̺�Ʈ�� ���� �� �ִ� �������̽���.
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
