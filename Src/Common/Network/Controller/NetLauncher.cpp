
#include "stdafx.h"
#include "NetLauncher.h"
#include "CoreClient.h"

using namespace network;


//------------------------------------------------------------------------
// ���� ����
//------------------------------------------------------------------------
bool CNetLauncher::LaunchServer(ServerPtr pSvr, int port)
{
	if (!pSvr)
		return false;

	// ������ �����ϰ� ������ Ȯ���մϴ�
	WORD wVersionRequested = MAKEWORD(1, 1);
	WSADATA wsaData;
	int nRet = WSAStartup(wVersionRequested, &wsaData);
	if(wsaData.wVersion != wVersionRequested)
	{
		error::ErrorLog( "���� ������ Ʋ�Ƚ��ϴ�" );
		return false;
	}

	// socket(�ּҰ迭, ���� ����, ��������)
	SOCKET svrSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(svrSocket == INVALID_SOCKET)
	{
		error::ErrorLog( "socket() error" );
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
		error::ErrorLog( "bind() error" );
		closesocket(svrSocket);
		return false;
	}

	int nLen;
	nLen = sizeof(SOCKADDR);
	char szBuf[256];

	nRet = gethostname( szBuf, sizeof(szBuf) );
	if (nRet == SOCKET_ERROR)
	{
		error::ErrorLog( "gethostname() error" );
		closesocket(svrSocket);
		return false;
	}

	// listen(���� ����, ��û ���� ������ �뷮)
	nRet = listen(svrSocket, SOMAXCONN);

	if (nRet == SOCKET_ERROR)
	{
		error::ErrorLog( "listen() error" );
		closesocket(svrSocket);
		return false;
	}

	pSvr->SetSocket(svrSocket);
	pSvr->SetPort(port);
	return true;
}


//------------------------------------------------------------------------
// Ŭ���̾�Ʈ ����
//------------------------------------------------------------------------
bool CNetLauncher::LaunchClient(ClientPtr pClient, const std::string &ip, int port)
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
bool	CNetLauncher::LaunchCoreClient(CoreClientPtr pClient, const std::string &ip, int port)
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
		error::ErrorLog( "���� ������ Ʋ�Ƚ��ϴ�" );
		return false;
	}

	LPHOSTENT lpHostEntry;
	lpHostEntry = gethostbyname(ip.c_str());
	if(lpHostEntry == NULL)
	{
		error::ErrorLog( "gethostbyname() error" );
		return false;
	}

	// TCP/IP ��Ʈ�� ������ �����մϴ�.
	// socket(�ּ� �迭, ���� ����, ��������
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		error::ErrorLog( "socket() error" );
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
		error::ErrorLog( "connect() error" );
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
