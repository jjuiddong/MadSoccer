//------------------------------------------------------------------------
// Name:    TaskWork.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// �ϳ��� ���ϰ� ����ϴ� ����� �Ѵ�.
//------------------------------------------------------------------------
#pragma once

#include "../DataStructure/PacketQueue.h"

namespace network
{
	DECLARE_TYPE_NAME_SCOPE(network, CTaskWork)
	class CTaskWork : public common::CTask
		, public sharedmemory::CSharedMem<CTaskWork, TYPE_NAME(network::CTaskWork)>
	{
	public:
		CTaskWork(netid netId, SOCKET sock):CTask(2,"TaskWork"), m_NetId(netId), m_Socket(sock) {}
		virtual ~CTaskWork() {}

	protected:
		netid	m_NetId;
		SOCKET m_Socket;

	public:
		virtual RUN_RESULT	Run() override
		{
			const timeval t = {0, 0}; // 0 millisecond
			fd_set readSockets;
			FD_ZERO(&readSockets);
			FD_SET(m_Socket, &readSockets);

			const int ret = select( readSockets.fd_count, &readSockets, NULL, NULL, &t );
			if (ret != 0 && ret != SOCKET_ERROR)
			{
				char buf[ CPacket::MAX_PACKETSIZE];
				const int result = recv( m_Socket, buf, sizeof(buf), 0);
				if (result == SOCKET_ERROR || result == 0) // ���� ��Ŷ����� 0�̸� ������ ����ٴ� �ǹ̴�.
				{
					CPacketQueue::Get()->PushPacket( 
						CPacketQueue::SPacketData(m_NetId, DisconnectPacket() ));
					return RR_END;
				}
				else
				{
					CPacketQueue::Get()->PushPacket( 
						CPacketQueue::SPacketData(m_NetId, CPacket(SERVER_NETID,buf)) );
				}
			}

			return RR_CONTINUE;
		}
	};

}
