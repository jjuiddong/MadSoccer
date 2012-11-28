
#include "stdafx.h"
#include "ServerThread.h"


using namespace network;



//------------------------------------------------------------------------
// 클라이언트가 새로 접속하는 것을 검사한다. 접속했다면 CServer에 알린다. 
//
//------------------------------------------------------------------------
void network::AcceptThread( void *pServerPtr )
{
	CServer *pSvr = (CServer*)pServerPtr;

	while (pSvr->IsServerOn())
	{
		// 오는 요청을 기다립니다.
		SOCKET remoteSocket;

		// accept(요청을 받으 소켓, 선택 클라이언트 주소)
		remoteSocket = accept(pSvr->GetListenSocket(), NULL, NULL);
		if(remoteSocket == INVALID_SOCKET)
		{
			//PRINTERROR("accept()");
			//closesocket(m_ListenSocket);
			break;
		}
		
		pSvr->EnterSync();
		pSvr->AddClient( remoteSocket );
		pSvr->LeaveSync();

		Sleep(10);
	}
}


//------------------------------------------------------------------------
// 클라이언트로 부터 패킷이 왔는지 검사한다. 패킷이 왔다면, 
// CServer에 패킷을 전달한다.
//------------------------------------------------------------------------
void network::RecvThread( void *pServerPtr )
{
	CServer *pSvr = (CServer*)pServerPtr;

	while (pSvr->IsServerOn())
	{
		const timeval t = {0, 10}; // 10 millisecond
		fd_set readSockets;

		pSvr->EnterSync();
		pSvr->MakeFDSET(&readSockets);
		pSvr->LeaveSync();

		const int ret = select( readSockets.fd_count, &readSockets, NULL, NULL, &t);
		if (ret != 0 && ret != SOCKET_ERROR)
		{
			for (u_int i=0; i < readSockets.fd_count; ++i)
			{
// 				if (!FD_ISSET(i, &readSockets)) continue;
				char buf[ 256];
				memset( buf, 0, sizeof(buf) );
				const int result = recv(readSockets.fd_array[ i], buf, sizeof(buf), 0);

				pSvr->EnterSync();
				if (result == INVALID_SOCKET)
				{
					pSvr->RemoveClient( readSockets.fd_array[ i] );
				}
				else
				{
					pSvr->PushPacket( CPacket(readSockets.fd_array[ i], buf) );
				}
				pSvr->LeaveSync();
			}
		}

		Sleep(1);
	}
}


//------------------------------------------------------------------------
// CServer에 저장된 패킷을 처리한다.
//------------------------------------------------------------------------
void network::WorkThread( void *pServerPtr )
{
	CServer *pSvr = (CServer*)pServerPtr;

	while (pSvr->IsServerOn())
	{
		pSvr->EnterSync();
		const PacketList &packets = pSvr->GetPackets();
		if (!packets.empty())
		{
			PacketCItor it = packets.begin();
			while (packets.end() != it)
			{
				pSvr->ProcessPacket( *it++ );
			}
			pSvr->ClearPackets();
		}
		pSvr->LeaveSync();

		Sleep(1);
	}
}
