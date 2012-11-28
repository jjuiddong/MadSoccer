
#include "stdafx.h"
#include "ServerThread.h"


using namespace network;



//------------------------------------------------------------------------
// Ŭ���̾�Ʈ�� ���� �����ϴ� ���� �˻��Ѵ�. �����ߴٸ� CServer�� �˸���. 
//
//------------------------------------------------------------------------
void network::AcceptThread( void *pServerPtr )
{
	CServer *pSvr = (CServer*)pServerPtr;

	while (pSvr->IsServerOn())
	{
		// ���� ��û�� ��ٸ��ϴ�.
		SOCKET remoteSocket;

		// accept(��û�� ���� ����, ���� Ŭ���̾�Ʈ �ּ�)
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
// Ŭ���̾�Ʈ�� ���� ��Ŷ�� �Դ��� �˻��Ѵ�. ��Ŷ�� �Դٸ�, 
// CServer�� ��Ŷ�� �����Ѵ�.
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
// CServer�� ����� ��Ŷ�� ó���Ѵ�.
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
