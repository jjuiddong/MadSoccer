
#include "stdafx.h"
#include "Server.h"
#include <winsock.h>


using namespace network;


CServer::CServer()
{
	m_ServerPort = 2333;

}


CServer::~CServer()
{
	Clear();
}


//------------------------------------------------------------------------
// 서버를 띄운다.
//------------------------------------------------------------------------
bool CServer::Start()
{
	WORD wVersionRequested = MAKEWORD(1, 1);
	WSADATA wsaData;
	int nRet;
//	short nPort;

	// 포트 번호를 확인 합니다.
// 	if (argc !=2)
// 	{
// 		fprintf(stderr, "\n사용법 : udp_server [포트 번호]\n");
// 		return 1;
// 	}

//	nPort = atoi(argv[1]);
//	nPort = 2333;

	// 윈속을 시작하고 버전을 확인합니다
	nRet = WSAStartup(wVersionRequested, &wsaData);
	if(wsaData.wVersion != wVersionRequested)
	{
//		fprintf(stderr, "\n 윈속 버전이 틀렸습니다.\n");
		return false;
	}

	// 스트림 서버가 할일을 수행합니다.
//	StreamServer(nPort);

  // TCP/IP 스트림 소켓을 "listen"으로 생성합니다.

//    SOCKET listenSocket;

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
//    int nRet;
    
    // bind(소켓, 서버 주소, 주소 구조체의 길이
    nRet = bind(m_ListenSocket, (LPSOCKADDR)&saServer, sizeof(struct sockaddr) );
    
    if(nRet == SOCKET_ERROR)
    {
//        PRINTERROR("bind()");
        closesocket(m_ListenSocket);
        return false;
    }

    /*
    이 부분은 잘 쓰이거나 요구되지 않습니다만, 이 예제에서 우리는 서버가 기다리고 있다는 것을
    표시해줘야지 클라이언트가 접속할수 있어서 표시합니다.
    */

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

    // 서버 주소와 포트 번호를 표시 합니다.
//    printf("\n %s 서버가 %d 포트에서 요청을 기다립니다. \n", szBuf, nPort);

    // 소켓에게 기다리게 합니다.

//    printf("\nlisten()");

    // listen(오는 소켓, 요청 수용 가능한 용량)
    nRet = listen(m_ListenSocket, SOMAXCONN);

    if(nRet == SOCKET_ERROR)
    {
//        PRINTERROR("listen()");
        closesocket(m_ListenSocket);
        return false;
    }

    // 오는 요청을 기다립니다.
    SOCKET remoteSocket;

//    printf("\naccept() 에서 막혔습니다.");

    // accept(요청을 받으 소켓, 선택 클라이언트 주소)
    remoteSocket = accept(m_ListenSocket, NULL, NULL);

    if(remoteSocket == INVALID_SOCKET)
    {
//        PRINTERROR("accept()");
        closesocket(m_ListenSocket);
        return false;
    }

	AddClient( remoteSocket );

    /*
    
       이제 클라이언트와 연결 했습니다.
       새로운 소켓 식별자가 준비 되었다고 클라이언트 주소를 되돌림
       값을 지고 왔습니다.

    */

    // 클라이언트에서 값을 받습니다.
    memset( szBuf, 0, sizeof(szBuf) );
    nRet = recv(remoteSocket, szBuf, sizeof(szBuf), 0);

    if (nRet == INVALID_SOCKET)
    {
//        PRINTERROR("recv()");
        closesocket(m_ListenSocket);
        closesocket(remoteSocket);
        return false;
    }

    // 받은 값을 표시 합니다.
//    printf("\n값을 받았습니다 : %s", szBuf);

    // 클라이언트에게 값을 되돌려 줍니다.
    strcpy_s(szBuf, "클라이언트로 값을 보냅니다. 잘받았습니다");

    // send(연결된 소켓, 보낼 버퍼, 버퍼의 길이, 상태값)
    nRet = send(remoteSocket, szBuf, sizeof(szBuf), 0);

    // 소켓 둘다 닫습니다
    closesocket(remoteSocket);
    closesocket(m_ListenSocket);

	// 소켓을 풉니다.
	WSACleanup();

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
void CServer::OnClientJoin()
{

}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CServer::OnClientLeave()
{

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CServer::Recv()
{

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CServer::Proc()
{

	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CServer::AddClient(SOCKET sock)
{
	SockItor it = find(m_SockList.begin(), m_SockList.end(), sock );
	if (m_SockList.end() != it)
		return false; //이미 존재한다면 실패

	m_SockList.push_back( sock );
	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CServer::RemoveClient(SOCKET sock)
{
	SockItor it = find(m_SockList.begin(), m_SockList.end(), sock );
	if (m_SockList.end() == it)
		return false; //없다면 실패

	m_SockList.remove(sock);
	return true;
}


//------------------------------------------------------------------------
// 모든 소켓 제거
//------------------------------------------------------------------------
void CServer::Clear()
{
	SockItor it = m_SockList.begin();
	while (m_SockList.end() != it)
	{
		SOCKET sock = *it++;
		closesocket(sock);
	}
	m_SockList.clear();
}

