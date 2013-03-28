
#include "stdafx.h"
#include "NetLauncher.h"
#include "CoreClient.h"

using namespace network;

//------------------------------------------------------------------------
// ���� ����
//------------------------------------------------------------------------
bool netlauncher::LaunchServer(ServerBasicPtr pSvr, int port)
{
	if (!pSvr)
		return false;

	// ������ �����ϰ� ������ Ȯ���մϴ�
	WORD wVersionRequested = MAKEWORD(1, 1);
	WSADATA wsaData;
	int nRet = WSAStartup(wVersionRequested, &wsaData);
	if(wsaData.wVersion != wVersionRequested)
	{
		clog::Error( clog::ERROR_CRITICAL,  "���� ������ Ʋ�Ƚ��ϴ�\n" );
		return false;
	}

	// socket(�ּҰ迭, ���� ����, ��������)
	SOCKET svrSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(svrSocket == INVALID_SOCKET)
	{
		clog::Error( clog::ERROR_CRITICAL, "socket() error\n" );
		return false;
	}

	// �ּ� ����ü�� ä��ϴ�.
	SOCKADDR_IN saServer;
	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = INADDR_ANY;    // ������ �����ϰ� �Ӵϴ�.
	saServer.sin_port = htons(port);		// ����ٿ��� ���� ��Ʈ�� �ֽ��ϴ�.

	// ���ϰ� listensocket �� bind(����) �մϴ�.
	// bind(����, ���� �ּ�, �ּ� ����ü�� ����
	nRet = bind(svrSocket, (LPSOCKADDR)&saServer, sizeof(struct sockaddr) );
	if(nRet == SOCKET_ERROR)
	{
		clog::Error( clog::ERROR_CRITICAL,  "bind() error\n" );
		closesocket(svrSocket);
		return false;
	}

	int nLen;
	nLen = sizeof(SOCKADDR);
	char szBuf[256];

	nRet = gethostname( szBuf, sizeof(szBuf) );
	if (nRet == SOCKET_ERROR)
	{
		clog::Error( clog::ERROR_CRITICAL, "gethostname() error\n" );
		closesocket(svrSocket);
		return false;
	}

	// listen(���� ����, ��û ���� ������ �뷮)
	nRet = listen(svrSocket, SOMAXCONN);

	if (nRet == SOCKET_ERROR)
	{
		clog::Error( clog::ERROR_CRITICAL, "listen() error\n" );
		closesocket(svrSocket);
		return false;
	}

	pSvr->SetSocket(svrSocket);
	pSvr->SetPort(port);
	pSvr->OnListen();
	return true;
}


//------------------------------------------------------------------------
// Ŭ���̾�Ʈ ����
//------------------------------------------------------------------------
bool netlauncher::LaunchClient(ClientPtr pClient, const std::string &ip, int port)
{
	if (!pClient)
		return false;

	if (!LaunchCoreClient(pClient->GetConnectSvrClient(), ip, port))
		return false;

	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool	netlauncher::LaunchCoreClient(CoreClientPtr pClient, const std::string &ip, int port)
{
	if (!pClient)
		return false;

	pClient->SetConnect(false);

	// ���� ������ Ȯ�� �մϴ�.
	WORD wVersionRequested = MAKEWORD(1,1);
	WSADATA wsaData;
	int nRet = WSAStartup(wVersionRequested, &wsaData);
	if (wsaData.wVersion != wVersionRequested)
	{
		clog::Error( clog::ERROR_CRITICAL, "���� ������ Ʋ�Ƚ��ϴ�\n" );
		return false;
	}

	LPHOSTENT lpHostEntry;
	lpHostEntry = gethostbyname(ip.c_str());
	if(lpHostEntry == NULL)
	{
		clog::Error( clog::ERROR_CRITICAL, "gethostbyname() error\n" );
		return false;
	}

	// TCP/IP ��Ʈ�� ������ �����մϴ�.
	// socket(�ּ� �迭, ���� ����, ��������
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		clog::Error( clog::ERROR_CRITICAL, "socket() error\n" );
		return false;
	}

	// �ּ� ����ü�� ä��ϴ�.
	SOCKADDR_IN saServer;
	saServer.sin_family = AF_INET;
	saServer.sin_addr = *((LPIN_ADDR)*lpHostEntry->h_addr_list); // ���� �ּ�
	saServer.sin_port = htons(port);

	// ������ �����մϴ�
	// connect(����, ���� �ּ�, ���� �ּ��� ����
	nRet = connect(clientSocket, (LPSOCKADDR)&saServer, sizeof(struct sockaddr) );
	if(nRet == SOCKET_ERROR)
	{
		clog::Error( clog::ERROR_CRITICAL, "connect() error\n" );
		closesocket(clientSocket);
		return false;
	}

	pClient->SetServerIp(ip);
	pClient->SetServerPort(port);
	pClient->SetSocket(clientSocket);
	pClient->SetConnect(true);
	pClient->OnConnect();

	return true;
}
