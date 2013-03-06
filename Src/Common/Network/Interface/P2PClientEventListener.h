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
		virtual void		OnMemberJoin(P2PClientPtr client) {}
		virtual void		OnMemberLeave(P2PClientPtr client) {}
	};

}
