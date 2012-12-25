
#include "stdafx.h"
#include "Client.h"


using namespace network;


CClient::CClient() :
	m_Id(common::GenerateId())
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
			OnDisconnect();
		}
		else
		{
			ProcessPacket( CPacket(m_Socket,buf) );
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
// ������ ����� ������ �� ȣ��ȴ�.
//------------------------------------------------------------------------
void CClient::OnDisconnect()
{
	m_IsConnect = false;

}


//------------------------------------------------------------------------
// ��Ŷ ����
//------------------------------------------------------------------------
bool CClient::Send(const CPacket &packet)
{
	// send(����� ����, ���� ����, ������ ����, ���°�)
	const int result = send(m_Socket, packet.GetData(), packet.GetPacketSize(), 0);
	if (result == INVALID_SOCKET)
	{
		OnDisconnect();
		return false;
	}
	return true;
}
