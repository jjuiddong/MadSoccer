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
		CTaskWorkServer(int taskId, netid netId);
		virtual ~CTaskWorkServer()
		{
			int a = 0;
		}
		virtual RUN_RESULT	Run() override;

	protected:
		netid m_ServerId;
	};


	inline CTaskWorkServer::CTaskWorkServer(int taskId, netid netId) :
		CTask(taskId,"TaskWorkServer"), m_ServerId(netId) 
	{
	}

	//------------------------------------------------------------------------
	// Run
	//------------------------------------------------------------------------
	inline common::CTask::RUN_RESULT CTaskWorkServer::Run()
	{
		ServerPtr psvr = GetServer(m_ServerId);
		if (!psvr) 
		{
			LogNPrint( "CTaskWorkServer::Run() Error!! not found server netid: %d", m_ServerId );
			return RR_END;
		}

		const timeval t = {0, 10}; // 10 millisecond
		SFd_Set readSockets;
		psvr->MakeFDSET(&readSockets);
		const SFd_Set sockets = readSockets;

		const int ret = select( readSockets.fd_count, &readSockets, NULL, NULL, &t);
		if (ret != 0 && ret != SOCKET_ERROR)
		{
			for (u_int i=0; i < sockets.fd_count; ++i)
			{
				if (!FD_ISSET(sockets.fd_array[ i], &readSockets)) continue;

				char buf[ CPacket::MAX_PACKETSIZE];
				const int result = recv(sockets.fd_array[ i], buf, sizeof(buf), 0);
				if (result == INVALID_SOCKET || 0 == result)
				{
					psvr->RemoveClient(sockets.netid_array[ i]);
				}
				else
				{
					const netid senderId = sockets.netid_array[ i];
					CPacketQueue::Get()->PushPacket( 
						CPacketQueue::SPacketData(m_ServerId, CPacket(senderId, buf)) );
				}
			}
		}
		return RR_CONTINUE;
	}

}
