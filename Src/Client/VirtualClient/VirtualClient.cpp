// VirtualClient.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include "NetCommon/basic_Protocol.h"
#include "NetCommon/basic_ProtocolListener.h"
#include "NetCommon/basic_Protocol.cpp"
#include "NetCommon/basic_ProtocolListener.cpp"
#include "NetCommon/login_Protocol.h"
#include "NetCommon/login_ProtocolListener.h"
#include "NetCommon/login_Protocol.cpp"
#include "NetCommon/login_ProtocolListener.cpp"


using namespace network;

class CVirtualClient :	public network::CClient
					 ,  public login::s2c_ProtocolListener
					 ,	public basic::s2c_ProtocolListener
{
public:
	CVirtualClient() {}

protected:
	virtual void ProcessPacket( const CPacket &rcvPacket )
	{
		//		printf( "recv %s\n", rcvPacket.GetData() );
	}

	virtual void AckLogin(netid senderId, const std::string &id, const int &result) override
	{
		printf( "id: %s, result: %d\n", id.c_str(), result );
	}
	virtual void func1(netid senderId) override
	{
		int a = 0;
		printf( "func1 recv senderId: %d\n", senderId );
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
	common::dump::InstallSelfMiniDump();

	try
	{
		char *p = NULL;
		*p = 'c';
	}
	catch(...)
	{

	}

	basic::c2s_Protocol protocol;
	login::c2s_Protocol login_protocol;
	CVirtualClient client;
	client.RegisterProtocol(&protocol);
	client.RegisterProtocol(&login_protocol);
	client.AddListener( &client );

	network::StartClient( "127.0.0.1", 2334, &client );

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

//			protocol.func2(SERVER_NETID, str);
			static bool flag = true;

			if (flag)
				login_protocol.ReqLogin(SERVER_NETID, "testLogin", "pass1234" );
			else
				protocol.func2(SERVER_NETID, "client send func2" );
			flag = !flag;
		}

		Sleep(1);
	}

	/**/
	return 0;
}
