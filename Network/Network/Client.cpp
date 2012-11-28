
#include "stdafx.h"
#include "Client.h"


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
// 클라이언트 시작
//------------------------------------------------------------------------
bool CClient::Start(std::string ip, int port)
{
	m_ServerIP = ip;
	m_ServerPort = port;

	// 윈속 버전을 확인 합니다.
	WORD wVersionRequested = MAKEWORD(1,1);
	WSADATA wsaData;
	int nRet = WSAStartup(wVersionRequested, &wsaData);
	if (wsaData.wVersion != wVersionRequested)
	{
//		fprintf(stderr, "\n 윈속 버전이 틀립니다.\n");
		return false;
	}

	LPHOSTENT lpHostEntry;
	lpHostEntry = gethostbyname(m_ServerIP.c_str());
	if(lpHostEntry == NULL)
	{
//		PRINTERROR("gethostbyname()");
		return false;
	}

	// TCP/IP 스트림 소켓을 생성합니다.
	// socket(주소 계열, 소켓 형태, 프로토콜
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_Socket == INVALID_SOCKET)
	{
//		PRINTERROR("socket()");
		return false;
	}

	// 주소 구조체를 채웁니다.
	SOCKADDR_IN saServer;
	saServer.sin_family = AF_INET;
	saServer.sin_addr = *((LPIN_ADDR)*lpHostEntry->h_addr_list); // 서버 주소
	saServer.sin_port = htons(m_ServerPort);

	// 서버로 접속합니다
	// connect(소켓, 서버 주소, 서버 주소의 길이
	nRet = connect(m_Socket, (LPSOCKADDR)&saServer, sizeof(struct sockaddr) );
	if(nRet == SOCKET_ERROR)
	{
//		PRINTERROR("socket()");
		closesocket(m_Socket);
		return false;
	}
	m_IsConnect = true;
	OnConnect();

	return true;
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
// 서버와 통신이 끊겼을 때 호출된다.
//------------------------------------------------------------------------
void CClient::OnDisconnect()
{
	m_IsConnect = false;

}


//------------------------------------------------------------------------
// 패킷 전송
//------------------------------------------------------------------------
bool CClient::Send(const CPacket &packet)
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
