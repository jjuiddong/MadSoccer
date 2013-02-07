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
		, public sharedmemory::CSharedMem<CTaskAccept, TYPE_NAME(network::CTaskAccept)>
	{
	public:
		CTaskAccept() : CTask(0,"TaskAccept") {}
		virtual ~CTaskAccept() {}

	public:
		virtual RUN_RESULT	Run() override
		{
			const timeval t = {0, 10}; // 10 millisecond
			fd_set readSockets;
			CNetController::Get()->MakeServersFDSET(&readSockets);

			const int ret = select( readSockets.fd_count, &readSockets, NULL, NULL, &t);
			if (ret != 0 && ret != SOCKET_ERROR)
			{
				for (u_int i=0; i < readSockets.fd_count; ++i)
				{
					// accept(��û�� ���� ����, ���� Ŭ���̾�Ʈ �ּ�)
					SOCKET remoteSocket = accept(readSockets.fd_array[ i], NULL, NULL);
					if (remoteSocket == INVALID_SOCKET)
					{
						error::ErrorLog( "Client�� Accept�ϴ� ���߿� ������ �߻���" );
						return RR_CONTINUE;
					}

					CServer *pSvr = CNetController::Get()->GetServer(readSockets.fd_array[ i]);
					if (!pSvr)
					{
						error::ErrorLog( 
							common::format("%d ���Ͽ� �ش��ϴ� Ŭ���̾�Ʈ�� ã������", 
								readSockets.fd_array[ i]) );
						return RR_CONTINUE;
					}

 					pSvr->EnterSync();
 					pSvr->AddClient( remoteSocket );
 					pSvr->LeaveSync();
				}
			}

			return RR_CONTINUE; 
		}
	};

}
