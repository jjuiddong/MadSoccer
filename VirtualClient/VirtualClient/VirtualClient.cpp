// VirtualClient.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "../../Common/Common/Common.h"
#include "../../Network/Network/Network.h"
#include <conio.h>
#include <stdio.h>
#include <iostream>

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

struct SA
{
	int a;
	char b;
	float d;
	double e;
};

const CPacket& operator<<(CPacket &lhs, const SA &rhs)
{
	lhs.Append(rhs);
	return lhs;
}



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
	 		char buf[ 8];
	 		gets_s(buf);
			const int protocol = atoi(buf);
			CPacket packet;
			packet << protocol;

			SA a;
			a.a = 100;
			packet << a;
			packet << 10;
			packet << 0.1f;

			char buf2[32] = {"continue String"};
			packet << 10 << 11 << buf2;

	 		client.Send( packet );
		}

		Sleep(1);
	}

	return 0;
}
