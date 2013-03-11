//------------------------------------------------------------------------
// Name:    AcceptTask.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// 클라이언트를 받아서 소켓을 서버에게 넘겨준다.
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
				// accept(요청을 받으 소켓, 선택 클라이언트 주소)
				SOCKET remoteSocket = accept(readSockets.fd_array[ i], NULL, NULL);
				if (remoteSocket == INVALID_SOCKET)
				{
					error::ErrorLog( "Client를 Accept하는 도중에 에러가 발생함" );
					return RR_CONTINUE;
				}

				CServer *pSvr = CNetController::Get()->GetServerFromSocket(readSockets.fd_array[ i]);
				if (!pSvr)
				{
					error::ErrorLog( 
						common::format("%d 소켓에 해당하는 클라이언트를 찾지못함", 
						readSockets.fd_array[ i]) );
					return RR_CONTINUE;
				}

				pSvr->AddClient( remoteSocket );
			}
		}
		return RR_CONTINUE; 
	}

}
