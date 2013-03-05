//------------------------------------------------------------------------
// Name:    TaskWorkServer.h
// Author:  jjuiddong
// Date:    03/04/2013
// 
// 서버와 연결된 클라이언트 패킷을 recv 한다.
// 클라이언트로부터 패킷을 받아서CPacketQueue에 저장한다.
//------------------------------------------------------------------------
#pragma once

#include "../DataStructure/PacketQueue.h"

namespace network
{
	DECLARE_TYPE_NAME_SCOPE(network, CTaskWorkServer)
	class CTaskWorkServer : public common::CTask
		, public sharedmemory::CSharedMem<CTaskWorkServer, TYPE_NAME(network::CTaskWorkServer)>
	{
	public:
		CTaskWorkServer(netid netId):CTask(4,"TaskWorkServer"), m_NetId(netId) {}
		virtual ~CTaskWorkServer() {}

	protected:
		netid		m_NetId;

	public:
		virtual RUN_RESULT	Run() override
		{
			ServerPtr psvr = GetServer(m_NetId);
			if (!psvr) 
			{
				LogNPrint( "CTaskWorkServer::Run() Error!! not found server netid: %d", m_NetId );
				return RR_END;
			}

			const timeval t = {0, 10}; // 10 millisecond
			SFd_Set readSockets;
			psvr->MakeFDSET(&readSockets);
			const SFd_Set sockets = readSockets;

			const int ret = select( readSockets.fd_count, &readSockets, NULL, NULL, &t);
			if (ret != 0 && ret != SOCKET_ERROR)
			{
				//for (u_int i=0; i < readSockets.fd_count; ++i)
				for (u_int i=0; i < sockets.fd_count; ++i)
				{
					if (!FD_ISSET(sockets.fd_array[ i], &readSockets)) continue;

					char buf[ CPacket::MAX_PACKETSIZE];
					const int result = recv(sockets.fd_array[ i], buf, sizeof(buf), 0);
					if (result == INVALID_SOCKET || 0 == result)
					{
						//m_pServer->RemoveClientBySocket(sockets.fd_array[ i]);
						psvr->RemoveClient(sockets.netid_array[ i]);
					}
					else
					{
						//const netid senderId = m_pServer->GetNetIdFromSocket(sockets.fd_array[ i]);
						const netid senderId = sockets.netid_array[ i];
						CPacketQueue::Get()->PushPacket( 
							CPacketQueue::SPacketData(m_NetId, CPacket(senderId, buf)) );
					}
				}
			}

			return RR_CONTINUE;
		}
	};

}
