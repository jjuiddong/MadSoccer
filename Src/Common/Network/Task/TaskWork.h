//------------------------------------------------------------------------
// Name:    RecvWork.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// 클라이언트로부터 패킷을 받아서CPacketQueue에 저장한다.
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
		CTaskWork(ServerPtr psvr):CTask(1,"TaskWork"), m_pServer(psvr) {}
		virtual ~CTaskWork() {}

	protected:
		ServerPtr	m_pServer;

	public:
		virtual RUN_RESULT	Run() override
		{
			if (!m_pServer) return RR_CONTINUE; 
			const timeval t = {0, 10}; // 10 millisecond
			fd_set readSockets;

			m_pServer->EnterSync();
			m_pServer->MakeFDSET(&readSockets);
			m_pServer->LeaveSync();

			const int ret = select( readSockets.fd_count, &readSockets, NULL, NULL, &t);
			if (ret != 0 && ret != SOCKET_ERROR)
			{
				for (u_int i=0; i < readSockets.fd_count; ++i)
				{
// 					if (!FD_ISSET(i, &readSockets)) continue;
					char buf[ 256];
					memset( buf, 0, sizeof(buf) );
					const int result = recv(readSockets.fd_array[ i], buf, sizeof(buf), 0);
					if (result == INVALID_SOCKET || 0 == result)
					{
						m_pServer->RemoveClientBySocket(readSockets.fd_array[ i]);
					}
					else
					{
						const netid netId = m_pServer->GetNetIdFromSocket(readSockets.fd_array[ i]);
						CPacketQueue::Get()->PushPacket( 
							CPacketQueue::SPacketData(m_pServer->GetSocket(),CPacket(netId, buf)) );
					}
				}
			}

			return RR_CONTINUE;
		}
	};

}
