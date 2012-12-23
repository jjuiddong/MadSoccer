//------------------------------------------------------------------------
// Name:    RecvTask.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// 클라이언트로부터 패킷을 받아서 서버에게 넘겨준다.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class CRecvTask : public common::CTask
	{
	public:
		CRecvTask(CServer *psvr):CTask(1), m_pServer(psvr) {}
		virtual ~CRecvTask() {}

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

					m_pServer->EnterSync();
					if (result == INVALID_SOCKET)
					{
						m_pServer->RemoveClient( readSockets.fd_array[ i] );
					}
					else
					{
						m_pServer->PushPacket( CPacket(readSockets.fd_array[ i], buf) );
					}
					m_pServer->LeaveSync();
				}
			}

			return RR_CONTINUE;
		}
	};

}
