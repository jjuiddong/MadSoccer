
#include "stdafx.h"
#include "Client.h"


using namespace network;


CClient::CClient() 
{
	m_ServerIP = "127.0.0.1";
	m_ServerPort = 2333;

}


CClient::~CClient() 
{

}



//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CClient::Start()
{
	WORD wVersionRequested = MAKEWORD(1,1);
	WSADATA wsaData;
	int nRet;
//	short nPort;

	// 호트명과 포트 번호를 확인합니다.
// 	if (argc != 3)
// 	{
// 		fprintf(stderr,"\n사용법 : udp_client [서버주소] [포트번호]\n");
// 		return 1;
// 	}

	//nPort = atoi(argv[2]);
//	nPort = 2333;

	// 윈속 버전을 확인 합니다.

	nRet = WSAStartup(wVersionRequested, &wsaData);
	if (wsaData.wVersion != wVersionRequested)
	{
//		fprintf(stderr, "\n 윈속 버전이 틀립니다.\n");
		return false;
	}

	// 스트림 클라이언트가 해야 할 일을 합니다.
	// 여기까지는 UDP 와 크게 차이가 없습니다.
///	StreamClient(argv[1], nPort);

	LPHOSTENT lpHostEntry;

	lpHostEntry = gethostbyname(m_ServerIP.c_str());
	if(lpHostEntry == NULL)
	{
//		PRINTERROR("gethostbyname()");
		return false;
	}

	// TCP/IP 스트림 소켓을 생성합니다.
	SOCKET theSocket;

	// socket(주소 계열, 소켓 형태, 프로토콜
	theSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (theSocket == INVALID_SOCKET)
	{
//		PRINTERROR("socket()");
		return false;
	}

	// 주소 구조체를 채웁니다.
	SOCKADDR_IN saServer;

	saServer.sin_family = AF_INET;
	saServer.sin_addr = *((LPIN_ADDR)*lpHostEntry->h_addr_list); // 서버 주소
	saServer.sin_port = htons(m_ServerPort);    // 명령줄에서 포트 번호를 받습니다.

	// 서버로 접속합니다

//	int nRet;

	// connect(소켓, 서버 주소, 서버 주소의 길이
	nRet = connect(theSocket, (LPSOCKADDR)&saServer, sizeof(struct sockaddr) );

	if(nRet == SOCKET_ERROR)
	{
//		PRINTERROR("socket()");
		closesocket(theSocket);
		return false;
	}

	// 서버로 자료를 보냅니다.

	char szBuf[256];
	strcpy_s(szBuf, "클라이언트에서 보내는 TCP 값입니다");

	// Windows 에서는 Unix 계열과 다르게 보낼때 write 대신에 send 를 씁니다.
	// send(연결된 소켓, 보낼 자료 버퍼, 자료의 길이, 상태값
	nRet = send(theSocket, szBuf, strlen(szBuf), 0);

	if (nRet == SOCKET_ERROR)
	{
//		PRINTERROR("send()");
//		closesocket(theSocket);
		return false;
	}

	// 응답을 기다립니다

	// Windows 에서는 Unix 계열과 다르레 받을때는 read 대신에 recv 를 씁니다.
	// recv(연결된 소켓, 받은 자료를 저장할 버퍼, 버퍼의 길이, 상태값)
	nRet = recv(theSocket, szBuf, sizeof(szBuf), 0);
	if (nRet == SOCKET_ERROR)
	{
// 		PRINTERROR("recv()");
// 		closesocket(theSocket);
		return false;
	}

	// 받은 값을 표시합니다.
//	printf("\n값을 받았습니다 : %s", szBuf);

	closesocket(theSocket);

	WSACleanup();

	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CClient::Stop()
{

	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CClient::Connect()
{

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CClient::OnMemberJoin()
{

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CClient::OnMemberLeave()
{

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CClient::Proc()
{

	return true;
}
