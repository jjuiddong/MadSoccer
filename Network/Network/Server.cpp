
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
// 서버를 띄운다.
//------------------------------------------------------------------------
bool CServer::Start(int port)
{
	m_ServerPort = port;

	// 윈속을 시작하고 버전을 확인합니다
	WORD wVersionRequested = MAKEWORD(1, 1);
	WSADATA wsaData;
	int nRet = WSAStartup(wVersionRequested, &wsaData);
	if(wsaData.wVersion != wVersionRequested)
	{
//		fprintf(stderr, "\n 윈속 버전이 틀렸습니다.\n");
		return false;
	}

    // socket(주소계열, 소켓 형식, 프로토콜)
    m_ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_ListenSocket == INVALID_SOCKET)
    {
//        PRINTERROR("socket()");
        return false;
    }

    // 주소 구조체를 채웁니다.
    SOCKADDR_IN saServer;
    saServer.sin_family = AF_INET;
    saServer.sin_addr.s_addr = INADDR_ANY;    // 윈속이 제공하게 둡니다.
    saServer.sin_port = htons(m_ServerPort);        // 명령줄에서 받은 포트를 넣습니다.

    // 소켓과 listensocket 을 bind(묶습) 합니다.
    // bind(소켓, 서버 주소, 주소 구조체의 길이
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

    // 소켓에게 기다리게 합니다.

    // listen(오는 소켓, 요청 수용 가능한 용량)
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
		return false; //이미 존재한다면 실패

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
		return false; //없다면 실패

	m_ClientSockets.remove(sock);
	OnClientLeave(sock);
	return true;
}


//------------------------------------------------------------------------
// m_ClientSockets 루프안에서 Client를 제거해야 될때 쓰이는 함수다.
// Client를 제거하고 다음을 가르키는 iterator를 반환한다.
//------------------------------------------------------------------------
SockItor CServer::RemoveClientInLoop(SOCKET sock)
{
	SockItor it = find(m_ClientSockets.begin(), m_ClientSockets.end(), sock );
	if (m_ClientSockets.end() == it)
		return m_ClientSockets.end(); //없다면 실패

	SockItor re = m_ClientSockets.erase(it);
	OnClientLeave(sock);
	return re;
}


//------------------------------------------------------------------------
// 모든 소켓 제거
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
// m_ClientSockets에 저장된 socket으로 fd_set을 생성한다. 
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
// 패킷을 저장한다.
//------------------------------------------------------------------------
bool CServer::PushPacket(const CPacket &packet)
{
	m_Packets.push_back( packet );
	return true;
}


//------------------------------------------------------------------------
// 패킷을 모두 제거한다.
//------------------------------------------------------------------------
void CServer::ClearPackets()
{
	m_Packets.clear();
}


//------------------------------------------------------------------------
// 패킷 처리
// 상속받는 클래스에서 오버라이딩해서 처리한다.
//------------------------------------------------------------------------
void CServer::ProcessPacket( const CPacket &rcvPacket )
{
	if (!IsExist(rcvPacket.GetSenderSocket()))
		return; // 해당 클라이언트가 없다면 리턴시킨다.
}


//------------------------------------------------------------------------
// 해당 socket이 sockets리스트에 존재한다면 true를 리턴한다.
//------------------------------------------------------------------------
bool CServer::IsExist(SOCKET socket)
{
	SockItor it = find(m_ClientSockets.begin(), m_ClientSockets.end(), socket);
	return m_ClientSockets.end() != it;
}


//------------------------------------------------------------------------
// 패킷 전송
//------------------------------------------------------------------------
bool CServer::Send(SOCKET sock, const CPacket &packet)
{
	// send(연결된 소켓, 보낼 버퍼, 버퍼의 길이, 상태값)
	const int result = send(sock, packet.GetData(), packet.GetPacketSize(), 0);
	if (result == INVALID_SOCKET)
	{
		RemoveClient(packet.GetSenderSocket());
		return false;
	}
	return true;
}


//------------------------------------------------------------------------
// 연결된 모든 클라이언트에게 메세지를 보낸다. 
//------------------------------------------------------------------------
bool CServer::SendAll(const CPacket &packet)
{
	SockItor it = m_ClientSockets.begin();
	while (m_ClientSockets.end() != it)
	{
		// send(연결된 소켓, 보낼 버퍼, 버퍼의 길이, 상태값)
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
// 동기화 시작
//------------------------------------------------------------------------
void CServer::EnterSync()
{
	EnterCriticalSection( &m_CriticalSection );
}


//------------------------------------------------------------------------
// 동기화 끝
//------------------------------------------------------------------------
void CServer::LeaveSync()
{
	LeaveCriticalSection( &m_CriticalSection );
}
