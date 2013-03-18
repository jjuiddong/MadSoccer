//------------------------------------------------------------------------
// Name:    P2PClientEventListener.h
// Author:  jjuiddong
// Date:    3/5/2013
// 
// P2PClient���� �߻��ϴ� �̺�Ʈ�� ���� �� �ִ� �������̽� ��.
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
