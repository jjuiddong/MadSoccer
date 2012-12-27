
#include "stdafx.h"
#include "Client.h"
#include "Protocol.h"


using namespace network;


CClient::CClient()
{
	m_ServerIP = "127.0.0.1";
	m_ServerPort = 2333;
	m_IsConnect = false;

}


CClient::~CClient() 
{
	Clear();

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CClient::Stop()
{
	closesocket(m_Socket);

	return true;
}


//------------------------------------------------------------------------
// 매 프레임마다 호출되어야 하는 함수다.
// 패킷이 서버로 부터 왔는지 검사한다.
//------------------------------------------------------------------------
bool CClient::Proc()
{
	if (!IsConnect())
		return false;

 	const timeval t = {0, 0}; // 0 millisecond
 	fd_set readSockets;
	readSockets.fd_count = 1;
	readSockets.fd_array[ 0] = m_Socket;
 	const int ret = select( readSockets.fd_count, &readSockets, NULL, NULL, &t);
 	if (ret != 0 && ret != SOCKET_ERROR)
 	{
		char buf[ 256];
		const int result = recv( readSockets.fd_array[ 0], buf, sizeof(buf), 0);
		if (result == SOCKET_ERROR || result == 0) // 받은 패킷사이즈가 0이면 서버와 끊겼다는 의미다.
		{
			OnDisconnect();
		}
		else
		{
			CPacket packet(SERVER_NETID,buf);

			const int protocolId = packet.GetProtocolId();
			const ProtocolListenerList &listeners = GetListeners( protocolId );
			if (listeners.empty())
			{
				error::ErrorLog( 
					common::format(" CClient::Proc():: %d 에 해당하는 프로토콜 리스너가 없습니다.", 
						protocolId) );
			}
			else
			{
				listeners.front()->Dispatch(packet, listeners);
			}
		}
	}

	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CClient::Clear()
{
	m_IsConnect = false;
	closesocket(m_Socket);
	WSACleanup();

}


//------------------------------------------------------------------------
// 서버와 통신이 끊겼을 때 호출된다.
//------------------------------------------------------------------------
void CClient::OnDisconnect()
{
	m_IsConnect = false;

}


//------------------------------------------------------------------------
// 패킷 전송
//------------------------------------------------------------------------
bool CClient::Send(netid netId, const CPacket &packet)
{
	// send(연결된 소켓, 보낼 버퍼, 버퍼의 길이, 상태값)
	const int result = send(m_Socket, packet.GetData(), packet.GetPacketSize(), 0);
	if (result == INVALID_SOCKET)
	{
		OnDisconnect();
		return false;
	}
	return true;
}


//------------------------------------------------------------------------
// 연결된 모든 클라이언트들에게 메세지를 보낸다.
//------------------------------------------------------------------------
bool CClient::SendAll(const CPacket &packet)
{
	// 아직 아무것도 없음
	return true;
}

