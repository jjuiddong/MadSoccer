// VirtualClient.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include "C2SProtocol.h"
#include "S2CProtocolListener.h"

using namespace network;

class CVirtualClient : public network::CClient, public CS2CProtocolListener
{
public:
	CVirtualClient() {}

protected:
	virtual void ProcessPacket( const CPacket &rcvPacket )
	{
//		printf( "recv %s\n", rcvPacket.GetData() );
	}

	virtual void func1(netid senderId) override
	{
		int a = 0;
	}
	virtual void func2(netid senderId, std::string &str) override
	{
		printf( "func2 senderId: %d, str: %s\n", senderId, str.c_str());
	}
	virtual void func3(netid senderId, float value) override
	{
		int a = 0;
	}
	virtual void func4(netid senderId) override
	{
		int a = 0;
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
	CC2SProtocol protocol;
	CVirtualClient client;
	client.SetProtocol(&protocol);
	client.AddListener(&client);

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
			std::string str;
			std::getline(std::cin, str);

			protocol.func(SERVER_NETID, str);
		}

		Sleep(1);
	}

	return 0;
}
