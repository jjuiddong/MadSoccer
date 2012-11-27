
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

	// ȣƮ��� ��Ʈ ��ȣ�� Ȯ���մϴ�.
// 	if (argc != 3)
// 	{
// 		fprintf(stderr,"\n���� : udp_client [�����ּ�] [��Ʈ��ȣ]\n");
// 		return 1;
// 	}

	//nPort = atoi(argv[2]);
//	nPort = 2333;

	// ���� ������ Ȯ�� �մϴ�.

	nRet = WSAStartup(wVersionRequested, &wsaData);
	if (wsaData.wVersion != wVersionRequested)
	{
//		fprintf(stderr, "\n ���� ������ Ʋ���ϴ�.\n");
		return false;
	}

	// ��Ʈ�� Ŭ���̾�Ʈ�� �ؾ� �� ���� �մϴ�.
	// ��������� UDP �� ũ�� ���̰� �����ϴ�.
///	StreamClient(argv[1], nPort);

	LPHOSTENT lpHostEntry;

	lpHostEntry = gethostbyname(m_ServerIP.c_str());
	if(lpHostEntry == NULL)
	{
//		PRINTERROR("gethostbyname()");
		return false;
	}

	// TCP/IP ��Ʈ�� ������ �����մϴ�.
	SOCKET theSocket;

	// socket(�ּ� �迭, ���� ����, ��������
	theSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (theSocket == INVALID_SOCKET)
	{
//		PRINTERROR("socket()");
		return false;
	}

	// �ּ� ����ü�� ä��ϴ�.
	SOCKADDR_IN saServer;

	saServer.sin_family = AF_INET;
	saServer.sin_addr = *((LPIN_ADDR)*lpHostEntry->h_addr_list); // ���� �ּ�
	saServer.sin_port = htons(m_ServerPort);    // ����ٿ��� ��Ʈ ��ȣ�� �޽��ϴ�.

	// ������ �����մϴ�

//	int nRet;

	// connect(����, ���� �ּ�, ���� �ּ��� ����
	nRet = connect(theSocket, (LPSOCKADDR)&saServer, sizeof(struct sockaddr) );

	if(nRet == SOCKET_ERROR)
	{
//		PRINTERROR("socket()");
		closesocket(theSocket);
		return false;
	}

	// ������ �ڷḦ �����ϴ�.

	char szBuf[256];
	strcpy_s(szBuf, "Ŭ���̾�Ʈ���� ������ TCP ���Դϴ�");

	// Windows ������ Unix �迭�� �ٸ��� ������ write ��ſ� send �� ���ϴ�.
	// send(����� ����, ���� �ڷ� ����, �ڷ��� ����, ���°�
	nRet = send(theSocket, szBuf, strlen(szBuf), 0);

	if (nRet == SOCKET_ERROR)
	{
//		PRINTERROR("send()");
//		closesocket(theSocket);
		return false;
	}

	// ������ ��ٸ��ϴ�

	// Windows ������ Unix �迭�� �ٸ��� �������� read ��ſ� recv �� ���ϴ�.
	// recv(����� ����, ���� �ڷḦ ������ ����, ������ ����, ���°�)
	nRet = recv(theSocket, szBuf, sizeof(szBuf), 0);
	if (nRet == SOCKET_ERROR)
	{
// 		PRINTERROR("recv()");
// 		closesocket(theSocket);
		return false;
	}

	// ���� ���� ǥ���մϴ�.
//	printf("\n���� �޾ҽ��ϴ� : %s", szBuf);

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
