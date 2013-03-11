//------------------------------------------------------------------------
// Name:    TaskClientWork.h
// Author:  jjuiddong
// Date:    03/03/2013
// 
// CoreClient 중에 PROCESS_TYPE이 SERVICE_SEPERATE_THREAD 인
// 객체만 패킷을 받아서 Queue에 넘겨준다.
//------------------------------------------------------------------------
#pragma once

#include "../DataStructure/PacketQueue.h"

namespace network
{
	DECLARE_TYPE_NAME_SCOPE(network, CTaskWorkClient)
	class CTaskWorkClient : public common::CTask
		, public sharedmemory::CSharedMem<CTaskWorkClient, TYPE_NAME(network::CTaskWorkClient)>
	{
	public:
		CTaskWorkClient() : CTask(3,"TaskWorkClient") {}
		virtual RUN_RESULT	Run() override;
	};


	//------------------------------------------------------------------------
	// Run
	//------------------------------------------------------------------------
	inline common::CTask::RUN_RESULT CTaskWorkClient::Run()
	{
		const timeval t = {0, 0}; // 0 millisecond
		SFd_Set readSockets;
		CNetController::Get()->MakeCoreClientsFDSET(SERVICE_SEPERATE_THREAD, &readSockets);
		const SFd_Set sockets = readSockets;

		const int ret = select( readSockets.fd_count, &readSockets, NULL, NULL, &t );
		if (ret != 0 && ret != SOCKET_ERROR)
		{
			for (u_int i=0; i < sockets.fd_count; ++i)
			{
				if (!FD_ISSET(sockets.fd_array[ i], &readSockets)) continue;

				const netid recvId = sockets.netid_array[ i];
				char buf[ CPacket::MAX_PACKETSIZE];
				const int result = recv(sockets.fd_array[ i], buf, sizeof(buf), 0);
				if (result == INVALID_SOCKET || 0 == result)
				{
					CPacketQueue::Get()->PushPacket( 
						CPacketQueue::SPacketData(recvId, DisconnectPacket() ));
				}
				else
				{
					CPacketQueue::Get()->PushPacket(
						CPacketQueue::SPacketData(recvId, CPacket(SERVER_NETID, buf)) );
				}
			}
		}
		return RR_CONTINUE;
	}

}
