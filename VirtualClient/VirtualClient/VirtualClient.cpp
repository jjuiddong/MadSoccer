// VirtualClient.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "../../Network/Network/Network.h"
#include <conio.h>
#include <stdio.h>


using namespace network;

class CVirtualClient : public network::CClient
{
public:
	CVirtualClient() {}

protected:
	virtual void ProcessPacket( const CPacket &rcvPacket )
	{
		printf( "recv %s\n", rcvPacket.GetData() );
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	CVirtualClient client;
	network::StartClient( "127.0.0.1", 2333, &client );
	if (client.IsConnect())
	{
		printf( "Connect\n" );
	}

	while (client.IsConnect())
	{
		client.Proc();

		if (_kbhit())
		{
	 		char buf[ 256];
	 		gets_s(buf);
	 		client.Send( CPacket(0, buf) );
		}

		Sleep(1);
	}

	return 0;
}
