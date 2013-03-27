
#include "stdafx.h"
#include "CoreClient.h"
#include "../Interface/Protocol.h"
#include "../Service/AllProtocolListener.h"
#include "NetController.h"
#include "../ProtocolHandler/BasicProtocolDispatcher.h"
#include "../DataStructure/PacketQueue.h"


using namespace network;

CCoreClient::CCoreClient(PROCESS_TYPE procType) :
	CNetConnector(procType)
,	m_pEventListener(NULL)
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
	//Disconnect();
	return true;
}


//------------------------------------------------------------------------
// �� �����Ӹ��� ȣ��Ǿ�� �ϴ� �Լ���.
// ��Ŷ�� ������ ���� �Դ��� �˻��Ѵ�.
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
		if (result == SOCKET_ERROR || result == 0) // ���� ��Ŷ����� 0�̸� ������ ����ٴ� �ǹ̴�.
		{
			CPacketQueue::Get()->PushPacket( 
				CPacketQueue::SPacketData(GetNetId(), DisconnectPacket(GetNetId()) ));
		}
		else
		{
			CPacketQueue::Get()->PushPacket( 
				CPacketQueue::SPacketData(GetNetId(), CPacket(SERVER_NETID,buf)) );
		}
	}

	/// Dispatch Packet
	DispatchPacket();

	return true;
}


/**
 @brief Packet ����
 */
void	CCoreClient::DispatchPacket()
{
	CPacketQueue::SPacketData packetData;
	if (!CPacketQueue::Get()->PopPacket(GetNetId(), packetData))
		return;

	const ProtocolListenerList &listeners = GetProtocolListeners();
	if (listeners.empty())
	{
		clog::Error( clog::ERROR_CRITICAL, " CClientCore::DispatchPacket() �������� �����ʰ� �����ϴ�. netid: %d", GetNetId());
	}
	else
	{
		// ��� ��Ŷ�� �޾Ƽ� ó���ϴ� �����ʿ��� ��Ŷ�� ������.
		all::Dispatcher allDispatcher;
		allDispatcher.Dispatch(packetData.packet, listeners);
		// 

		const int protocolId = packetData.packet.GetProtocolId();

		// �⺻ �������� ó��
		if (protocolId == 0)
		{
			basic_protocol::ClientDispatcher dispatcher;
			dispatcher.Dispatch( packetData.packet, this );
			return;
		}

		IProtocolDispatcher *pDispatcher = CNetController::Get()->GetDispatcher(protocolId);
		if (!pDispatcher)
		{
			clog::Error( clog::ERROR_WARNING, 
				common::format(" CClientCore::DispatchPacket() %d �� �ش��ϴ� �������� �����İ� �����ϴ�.", 
				protocolId) );
		}
		else
		{
			pDispatcher->Dispatch(packetData.packet, listeners);
		}
	}

}


//------------------------------------------------------------------------
// ������ ������ �������� ȣ��ȴ�.
//------------------------------------------------------------------------
void CCoreClient::Disconnect()
{
	m_IsConnect = false;
	CNetController::Get()->RemoveCoreClient(this);
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


/**
 @brief 
 */
bool	CCoreClient::Send(netid netId, const SEND_FLAG flag, const CPacket &packet)
{
	// send(����� ����, ���� ����, ������ ����, ���°�)
	const int result = send(m_Socket, packet.GetData(), CPacket::MAX_PACKETSIZE, 0);
	if (result == INVALID_SOCKET)
	{
		Disconnect();
		return false;
	}
	return true;
}


//------------------------------------------------------------------------
// ����� ��� Ŭ���̾�Ʈ�鿡�� �޼����� ������.
//------------------------------------------------------------------------
bool CCoreClient::SendAll(const CPacket &packet)
{
	// ���� �ƹ��͵� ����
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
