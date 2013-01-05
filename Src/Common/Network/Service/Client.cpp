
#include "stdafx.h"
#include "Client.h"
#include "../Interface/Protocol.h"
#include "../Controller/NetController.h"
#include "AllProtocolListener.h"


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
	Disconnect();

	return true;
}


//------------------------------------------------------------------------
// �� �����Ӹ��� ȣ��Ǿ�� �ϴ� �Լ���.
// ��Ŷ�� ������ ���� �Դ��� �˻��Ѵ�.
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
		if (result == SOCKET_ERROR || result == 0) // ���� ��Ŷ����� 0�̸� ������ ����ٴ� �ǹ̴�.
		{
			Disconnect();
		}
		else
		{
			const ProtocolListenerList &listeners = GetListeners();
			if (listeners.empty())
			{
				error::ErrorLog( " CClient::Proc():: �������� �����ʰ� �����ϴ�.");
			}
			else
			{
				CPacket packet(SERVER_NETID,buf);

				// ��� ��Ŷ�� �޾Ƽ� ó���ϴ� �����ʿ��� ��Ŷ�� ������.
				all::Dispatcher allDispatcher;
				allDispatcher.Dispatch(packet, listeners);
				// 

				const int protocolId = packet.GetProtocolId();
				IProtocolDispatcher *pDispatcher = CNetController::Get()->GetDispatcher(protocolId);
				if (!pDispatcher)
				{
					error::ErrorLog( 
						common::format(" CClient::Proc() %d �� �ش��ϴ� �������� �����İ� �����ϴ�.", 
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
// ������ ������ �������� ȣ��ȴ�.
//------------------------------------------------------------------------
void CClient::Disconnect()
{
	m_IsConnect = false;
	closesocket(m_Socket);
	WSACleanup();

	OnDisconnect();
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
// ��Ŷ ����
//------------------------------------------------------------------------
bool CClient::Send(netid netId, const CPacket &packet)
{
	// send(����� ����, ���� ����, ������ ����, ���°�)
	const int result = send(m_Socket, packet.GetData(), packet.GetPacketSize(), 0);
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
bool CClient::SendAll(const CPacket &packet)
{
	// ���� �ƹ��͵� ����
	return true;
}

