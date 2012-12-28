// VirtualClient.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include "NetCommon/basic_Protocol.h"
#include "NetCommon/basic_ProtocolListener.h"
#include "NetCommon/basic_Protocol.cpp"
#include "NetCommon/basic_ProtocolListener.cpp"

using namespace network;

class CVirtualClient : public network::CClient, public basic::s2c_ProtocolListener
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
	virtual void func2(netid senderId, const std::string &str) override
	{
		printf( "func2 senderId: %d, str: %s\n", senderId, str.c_str());
	}
	virtual void func3(netid senderId, const float &value) override
	{
		int a = 0;
	}
	virtual void func4(netid senderId) override
	{
		int a = 0;
	}
};
/**/

struct SA
{
	int a;
	char b;
	float d;
	double e;

	operator char ()
	{ 
		return b;
	}
};

const CPacket& operator<<(CPacket &lhs, const SA &rhs)
{
	lhs.Append(rhs);
	return lhs;
}

int _tmain(int argc, _TCHAR* argv[])
{
	c2s_Protocol protocol;
	CVirtualClient client;
	client.RegisterProtocol(&protocol);
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

			protocol.func2(SERVER_NETID, str);
		}

		Sleep(1);
	}

	/**/
	return 0;
}
