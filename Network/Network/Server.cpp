
#include "stdafx.h"
#include "Server.h"
#include <winsock.h>
#include <process.h> 
#include "ServerThread.h"



using namespace network;


CServer::CServer() :
	m_IsServerOn(true)
{
	m_ServerPort = 2333;
	InitializeCriticalSection( &m_CriticalSection );

}


CServer::~CServer()
{
	Clear();
}


//------------------------------------------------------------------------
// ������ ����.
//------------------------------------------------------------------------
bool CServer::Start(int port)
{
	m_ServerPort = port;

	// ������ �����ϰ� ������ Ȯ���մϴ�
	WORD wVersionRequested = MAKEWORD(1, 1);
	WSADATA wsaData;
	int nRet = WSAStartup(wVersionRequested, &wsaData);
	if(wsaData.wVersion != wVersionRequested)
	{
//		fprintf(stderr, "\n ���� ������ Ʋ�Ƚ��ϴ�.\n");
		return false;
	}

    // socket(�ּҰ迭, ���� ����, ��������)
    m_ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_ListenSocket == INVALID_SOCKET)
    {
//        PRINTERROR("socket()");
        return false;
    }

    // �ּ� ����ü�� ä��ϴ�.
    SOCKADDR_IN saServer;
    saServer.sin_family = AF_INET;
    saServer.sin_addr.s_addr = INADDR_ANY;    // ������ �����ϰ� �Ӵϴ�.
    saServer.sin_port = htons(m_ServerPort);        // ����ٿ��� ���� ��Ʈ�� �ֽ��ϴ�.

    // ���ϰ� listensocket �� bind(����) �մϴ�.
    // bind(����, ���� �ּ�, �ּ� ����ü�� ����
    nRet = bind(m_ListenSocket, (LPSOCKADDR)&saServer, sizeof(struct sockaddr) );
    if(nRet == SOCKET_ERROR)
    {
//        PRINTERROR("bind()");
        closesocket(m_ListenSocket);
        return false;
    }

    int nLen;
    nLen = sizeof(SOCKADDR);
    char szBuf[256];

    nRet = gethostname( szBuf, sizeof(szBuf) );
    if (nRet == SOCKET_ERROR)
    {
//        PRINTERROR("gethostname()");
        closesocket(m_ListenSocket);
        return false;
    }

    // ���Ͽ��� ��ٸ��� �մϴ�.

    // listen(���� ����, ��û ���� ������ �뷮)
    nRet = listen(m_ListenSocket, SOMAXCONN);

    if(nRet == SOCKET_ERROR)
    {
//        PRINTERROR("listen()");
        closesocket(m_ListenSocket);
        return false;
    }

	_beginthread( AcceptThread, 0, this );

	_beginthread( RecvThread, 0, this );

	_beginthread( WorkThread, 0, this );

	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CServer::Stop()
{

	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CServer::OnListen()
{

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CServer::AddClient(SOCKET sock)
{
	SockItor it = find(m_ClientSockets.begin(), m_ClientSockets.end(), sock );
	if (m_ClientSockets.end() != it)
		return false; //�̹� �����Ѵٸ� ����

	m_ClientSockets.push_back( sock );
	OnClientJoin(sock);
	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CServer::RemoveClient(SOCKET sock)
{
	SockItor it = find(m_ClientSockets.begin(), m_ClientSockets.end(), sock );
	if (m_ClientSockets.end() == it)
		return false; //���ٸ� ����

	m_ClientSockets.remove(sock);
	OnClientLeave(sock);
	return true;
}


//------------------------------------------------------------------------
// m_ClientSockets �����ȿ��� Client�� �����ؾ� �ɶ� ���̴� �Լ���.
// Client�� �����ϰ� ������ ����Ű�� iterator�� ��ȯ�Ѵ�.
//------------------------------------------------------------------------
SockItor CServer::RemoveClientInLoop(SOCKET sock)
{
	SockItor it = find(m_ClientSockets.begin(), m_ClientSockets.end(), sock );
	if (m_ClientSockets.end() == it)
		return m_ClientSockets.end(); //���ٸ� ����

	SockItor re = m_ClientSockets.erase(it);
	OnClientLeave(sock);
	return re;
}


//------------------------------------------------------------------------
// ��� ���� ����
//------------------------------------------------------------------------
void CServer::Clear()
{
	m_IsServerOn = false;
	Sleep(100);

	SockItor it = m_ClientSockets.begin();
	while (m_ClientSockets.end() != it)
	{
		SOCKET sock = *it++;
		closesocket(sock);
	}
	m_ClientSockets.clear();

	DeleteCriticalSection( &m_CriticalSection );
	closesocket(m_ListenSocket);
	WSACleanup();
}


//------------------------------------------------------------------------
// m_ClientSockets�� ����� socket���� fd_set�� �����Ѵ�. 
//------------------------------------------------------------------------
void CServer::MakeFDSET( fd_set *pfdset)
{
	if (!pfdset)
		return;

	FD_ZERO(pfdset);
	SockItor it = m_ClientSockets.begin();
	while (m_ClientSockets.end() != it)
	{
		pfdset->fd_array[ pfdset->fd_count] = *it++;
		pfdset->fd_count++;
	}
}


//------------------------------------------------------------------------
// ��Ŷ�� �����Ѵ�.
//------------------------------------------------------------------------
bool CServer::PushPacket(const CPacket &packet)
{
	m_Packets.push_back( packet );
	return true;
}


//------------------------------------------------------------------------
// ��Ŷ�� ��� �����Ѵ�.
//------------------------------------------------------------------------
void CServer::ClearPackets()
{
	m_Packets.clear();
}


//------------------------------------------------------------------------
// ��Ŷ ó��
// ��ӹ޴� Ŭ�������� �������̵��ؼ� ó���Ѵ�.
//------------------------------------------------------------------------
void CServer::ProcessPacket( const CPacket &rcvPacket )
{
	if (!IsExist(rcvPacket.GetSenderSocket()))
		return; // �ش� Ŭ���̾�Ʈ�� ���ٸ� ���Ͻ�Ų��.
}


//------------------------------------------------------------------------
// �ش� socket�� sockets����Ʈ�� �����Ѵٸ� true�� �����Ѵ�.
//------------------------------------------------------------------------
bool CServer::IsExist(SOCKET socket)
{
	SockItor it = find(m_ClientSockets.begin(), m_ClientSockets.end(), socket);
	return m_ClientSockets.end() != it;
}


//------------------------------------------------------------------------
// ��Ŷ ����
//------------------------------------------------------------------------
bool CServer::Send(SOCKET sock, const CPacket &packet)
{
	// send(����� ����, ���� ����, ������ ����, ���°�)
	const int result = send(sock, packet.GetData(), packet.GetPacketSize(), 0);
	if (result == INVALID_SOCKET)
	{
		RemoveClient(packet.GetSenderSocket());
		return false;
	}
	return true;
}


//------------------------------------------------------------------------
// ����� ��� Ŭ���̾�Ʈ���� �޼����� ������. 
//------------------------------------------------------------------------
bool CServer::SendAll(const CPacket &packet)
{
	SockItor it = m_ClientSockets.begin();
	while (m_ClientSockets.end() != it)
	{
		// send(����� ����, ���� ����, ������ ����, ���°�)
		const int result = send(*it, packet.GetData(), packet.GetPacketSize(), 0);
		if (result == INVALID_SOCKET)
		{
			it = RemoveClientInLoop(*it);
		}
		else
		{
			++it;
		}
	}

	return true;
}


//------------------------------------------------------------------------
// ����ȭ ����
//------------------------------------------------------------------------
void CServer::EnterSync()
{
	EnterCriticalSection( &m_CriticalSection );
}


//------------------------------------------------------------------------
// ����ȭ ��
//------------------------------------------------------------------------
void CServer::LeaveSync()
{
	LeaveCriticalSection( &m_CriticalSection );
}
