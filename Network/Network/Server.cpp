
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
// ������ ����.
//------------------------------------------------------------------------
bool CServer::Start()
{
	WORD wVersionRequested = MAKEWORD(1, 1);
	WSADATA wsaData;
	int nRet;
//	short nPort;

	// ��Ʈ ��ȣ�� Ȯ�� �մϴ�.
// 	if (argc !=2)
// 	{
// 		fprintf(stderr, "\n���� : udp_server [��Ʈ ��ȣ]\n");
// 		return 1;
// 	}

//	nPort = atoi(argv[1]);
//	nPort = 2333;

	// ������ �����ϰ� ������ Ȯ���մϴ�
	nRet = WSAStartup(wVersionRequested, &wsaData);
	if(wsaData.wVersion != wVersionRequested)
	{
//		fprintf(stderr, "\n ���� ������ Ʋ�Ƚ��ϴ�.\n");
		return false;
	}

	// ��Ʈ�� ������ ������ �����մϴ�.
//	StreamServer(nPort);

  // TCP/IP ��Ʈ�� ������ "listen"���� �����մϴ�.

//    SOCKET listenSocket;

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
//    int nRet;
    
    // bind(����, ���� �ּ�, �ּ� ����ü�� ����
    nRet = bind(m_ListenSocket, (LPSOCKADDR)&saServer, sizeof(struct sockaddr) );
    
    if(nRet == SOCKET_ERROR)
    {
//        PRINTERROR("bind()");
        closesocket(m_ListenSocket);
        return false;
    }

    /*
    �� �κ��� �� ���̰ų� �䱸���� �ʽ��ϴٸ�, �� �������� �츮�� ������ ��ٸ��� �ִٴ� ����
    ǥ��������� Ŭ���̾�Ʈ�� �����Ҽ� �־ ǥ���մϴ�.
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

    // ���� �ּҿ� ��Ʈ ��ȣ�� ǥ�� �մϴ�.
//    printf("\n %s ������ %d ��Ʈ���� ��û�� ��ٸ��ϴ�. \n", szBuf, nPort);

    // ���Ͽ��� ��ٸ��� �մϴ�.

//    printf("\nlisten()");

    // listen(���� ����, ��û ���� ������ �뷮)
    nRet = listen(m_ListenSocket, SOMAXCONN);

    if(nRet == SOCKET_ERROR)
    {
//        PRINTERROR("listen()");
        closesocket(m_ListenSocket);
        return false;
    }

    // ���� ��û�� ��ٸ��ϴ�.
    SOCKET remoteSocket;

//    printf("\naccept() ���� �������ϴ�.");

    // accept(��û�� ���� ����, ���� Ŭ���̾�Ʈ �ּ�)
    remoteSocket = accept(m_ListenSocket, NULL, NULL);

    if(remoteSocket == INVALID_SOCKET)
    {
//        PRINTERROR("accept()");
        closesocket(m_ListenSocket);
        return false;
    }

	AddClient( remoteSocket );

    /*
    
       ���� Ŭ���̾�Ʈ�� ���� �߽��ϴ�.
       ���ο� ���� �ĺ��ڰ� �غ� �Ǿ��ٰ� Ŭ���̾�Ʈ �ּҸ� �ǵ���
       ���� ���� �Խ��ϴ�.

    */

    // Ŭ���̾�Ʈ���� ���� �޽��ϴ�.
    memset( szBuf, 0, sizeof(szBuf) );
    nRet = recv(remoteSocket, szBuf, sizeof(szBuf), 0);

    if (nRet == INVALID_SOCKET)
    {
//        PRINTERROR("recv()");
        closesocket(m_ListenSocket);
        closesocket(remoteSocket);
        return false;
    }

    // ���� ���� ǥ�� �մϴ�.
//    printf("\n���� �޾ҽ��ϴ� : %s", szBuf);

    // Ŭ���̾�Ʈ���� ���� �ǵ��� �ݴϴ�.
    strcpy_s(szBuf, "Ŭ���̾�Ʈ�� ���� �����ϴ�. �߹޾ҽ��ϴ�");

    // send(����� ����, ���� ����, ������ ����, ���°�)
    nRet = send(remoteSocket, szBuf, sizeof(szBuf), 0);

    // ���� �Ѵ� �ݽ��ϴ�
    closesocket(remoteSocket);
    closesocket(m_ListenSocket);

	// ������ Ǳ�ϴ�.
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
		return false; //�̹� �����Ѵٸ� ����

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
		return false; //���ٸ� ����

	m_SockList.remove(sock);
	return true;
}


//------------------------------------------------------------------------
// ��� ���� ����
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

