
#include "stdafx.h"
#include "CoreClient.h"
#include "../Interface/Protocol.h"
#include "../Service/AllProtocolListener.h"
#include "NetController.h"


using namespace network;

CCoreClient::CCoreClient(PROCESS_TYPE procType) :
	m_ServiceType(procType)
,	m_pEventListener(NULL)
,	m_hThread(NULL)
{
	m_ServerIP = "127.0.0.1";
	m_ServerPort = 2333;
	m_IsConnect = false;
}

CCoreClient::~CCoreClient() 
{
	Clear();
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CCoreClient::Stop()
{
	CNetController::Get()->StopCoreClient(this);
	Disconnect();
	return true;
}


//------------------------------------------------------------------------
// 매 프레임마다 호출되어야 하는 함수다.
// 패킷이 서버로 부터 왔는지 검사한다.
//------------------------------------------------------------------------
bool CCoreClient::Proc()
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
		char buf[ CPacket::MAX_PACKETSIZE];
		const int result = recv( readSockets.fd_array[ 0], buf, sizeof(buf), 0);
		if (result == SOCKET_ERROR || result == 0) // 받은 패킷사이즈가 0이면 서버와 끊겼다는 의미다.
		{
			Disconnect();
		}
		else
		{
			const ProtocolListenerList &listeners = GetListeners();
			if (listeners.empty())
			{
				error::ErrorLog( " CClientCore::Proc():: 프로토콜 리스너가 없습니다.");
			}
			else
			{
				CPacket packet(SERVER_NETID,buf);

				// 모든 패킷을 받아서 처리하는 리스너에게 패킷을 보낸다.
				all::Dispatcher allDispatcher;
				allDispatcher.Dispatch(packet, listeners);
				// 

				const int protocolId = packet.GetProtocolId();
				IProtocolDispatcher *pDispatcher = CNetController::Get()->GetDispatcher(protocolId);
				if (!pDispatcher)
				{
					error::ErrorLog( 
						common::format(" CClientCore::Proc() %d 에 해당하는 프로토콜 디스패쳐가 없습니다.", 
						protocolId) );
				}
				else
				{
					pDispatcher->Dispatch(packet, listeners);
				}
			}
		}
	}

	return true;
}


//------------------------------------------------------------------------
// 서버와 접속이 끊어질때 호출된다.
//------------------------------------------------------------------------
void CCoreClient::Disconnect()
{
	m_IsConnect = false;
	ClearConnection();
	OnDisconnect();
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CCoreClient::Clear()
{
	m_IsConnect = false;
	ClearConnection();

}


//------------------------------------------------------------------------
// 패킷 전송
//------------------------------------------------------------------------
bool CCoreClient::Send(netid netId, const CPacket &packet)
{
	// send(연결된 소켓, 보낼 버퍼, 버퍼의 길이, 상태값)
	const int result = send(m_Socket, packet.GetData(), packet.GetPacketSize(), 0);
	if (result == INVALID_SOCKET)
	{
		Disconnect();
		return false;
	}
	return true;
}


//------------------------------------------------------------------------
// 연결된 모든 클라이언트들에게 메세지를 보낸다.
//------------------------------------------------------------------------
bool CCoreClient::SendAll(const CPacket &packet)
{
	// 아직 아무것도 없음
	return true;
}


//------------------------------------------------------------------------
// Event Connect
//------------------------------------------------------------------------
void	CCoreClient::OnConnect()
{
	RET(!m_pEventListener);
	m_pEventListener->OnCoreClientConnect(this);
}


//------------------------------------------------------------------------
// Event Disconnect
//------------------------------------------------------------------------
void	CCoreClient::OnDisconnect()
{
	RET(!m_pEventListener);
	m_pEventListener->OnClientDisconnect(this);
}
