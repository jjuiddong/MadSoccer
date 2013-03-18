//------------------------------------------------------------------------
// Name:    ClientEventListener.h
// Author:  jjuiddong
// Date:    3/5/2013
// 
// CClient���� �߻��Ǵ� �̺�Ʈ�� ���� �� �ִ� ������ �������̽� ��.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class IClientEventListener
	{
	public:
		virtual ~IClientEventListener() {}

		// Server / Client Communication
		virtual void		OnClientConnect(ClientBasicPtr client) {}
		virtual void		OnClientDisconnect(ClientBasicPtr client) {}

		// P2P Communication
		virtual void		OnP2PConnect(ClientBasicPtr client) {}
		virtual void		OnP2PDisconnect(ClientBasicPtr client) {}

		virtual void		OnMemberJoin(ClientBasicPtr client, netid clientId) {}
		virtual void		OnMemberLeave(ClientBasicPtr client, netid clientId) {}
	};
}
