//------------------------------------------------------------------------
// Name:    AcceptTask.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// Ŭ���̾�Ʈ�� �޾Ƽ� ������ �������� �Ѱ��ش�.
//------------------------------------------------------------------------
#pragma once

#include "NetController.h"

namespace network
{
	DECLARE_TYPE_NAME_SCOPE(network, CTaskAccept)
	class CTaskAccept : public common::CTask
		, public memmonitor::Monitor<CTaskAccept, TYPE_NAME(network::CTaskAccept)>
	{
	public:
		CTaskAccept();
		virtual RUN_RESULT	Run() override;
	};


	inline CTaskAccept::CTaskAccept() : 
		CTask(0,"TaskAccept") 
	{
	}

	//------------------------------------------------------------------------
	// Run
	//------------------------------------------------------------------------
	inline common::CTask::RUN_RESULT CTaskAccept::Run()
	{
		const timeval t = {0, 10}; // 10 millisecond
		fd_set readSockets;
		CNetController::Get()->MakeServersFDSET(&readSockets);

		const int ret = select( readSockets.fd_count, &readSockets, NULL, NULL, &t );
		if (ret != 0 && ret != SOCKET_ERROR)
		{
			for (u_int i=0; i < readSockets.fd_count; ++i)
			{
				// accept(��û�� ���� ����, ���� Ŭ���̾�Ʈ �ּ�)
				SOCKET remoteSocket = accept(readSockets.fd_array[ i], NULL, NULL);
				if (remoteSocket == INVALID_SOCKET)
				{
					clog::Error( clog::ERROR_CRITICAL, "Client�� Accept�ϴ� ���߿� ������ �߻���\n" );
					return RR_CONTINUE;
				}

				CServerBasic *pSvr = CNetController::Get()->GetServerFromSocket(readSockets.fd_array[ i]);
				if (!pSvr)
				{
					clog::Error( clog::ERROR_PROBLEM,
						common::format("%d ���Ͽ� �ش��ϴ� ������ ã������\n", 
						readSockets.fd_array[ i]) );
					return RR_CONTINUE;
				}

				// get ip address
				sockaddr_in addr;
				int len = sizeof(addr);
				memset(&addr,0,sizeof(addr));
				getpeername( remoteSocket, (sockaddr*)&addr, &len );
				std::string ip = inet_ntoa(addr.sin_addr);

				//pSvr->AddSession( remoteSocket, ip );
				CPacketQueue::Get()->PushPacket( 
					CPacketQueue::SPacketData(pSvr->GetNetId(), AcceptPacket(remoteSocket, ip)) );
			}
		}
		return RR_CONTINUE; 
	}

}
