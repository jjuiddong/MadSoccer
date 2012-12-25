//------------------------------------------------------------------------
// Name:    RecvWork.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// 클라이언트로부터 패킷을 받아서 서버에게 넘겨준다.
//------------------------------------------------------------------------
#pragma once

#include "PacketQueue.h"

namespace network
{
	class CTaskWork : public common::CTask
	{
	public:
		CTaskWork(CServer *psvr):CTask(1), m_pServer(psvr) {}
		virtual ~CTaskWork() {}

	protected:
		common::ReferencePtr<CServer>	m_pServer;

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

					CPacketQueue::Get()->PushPacket( 
						CPacketQueue::SPacketData(m_pServer->GetSocket(),
							CPacket(readSockets.fd_array[ i], buf)));
				}
			}

			return RR_CONTINUE;
		}
	};

}
