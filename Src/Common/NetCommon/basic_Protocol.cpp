#include "stdafx.h"
#include "basic_Protocol.h"
using namespace network;
using namespace basic;

//------------------------------------------------------------------------
// Protocol: func1
//------------------------------------------------------------------------
void s2c_Protocol::func1(netid targetId)
{
	CPacket packet;
	packet << GetId();
	packet << 200;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: func2
//------------------------------------------------------------------------
void s2c_Protocol::func2(netid targetId, const std::string &str)
{
	CPacket packet;
	packet << GetId();
	packet << 201;
	packet << str;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: func3
//------------------------------------------------------------------------
void s2c_Protocol::func3(netid targetId, const float &value)
{
	CPacket packet;
	packet << GetId();
	packet << 202;
	packet << value;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: func4
//------------------------------------------------------------------------
void s2c_Protocol::func4(netid targetId)
{
	CPacket packet;
	packet << GetId();
	packet << 203;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: func5
//------------------------------------------------------------------------
void s2c_Protocol::func5(netid targetId, const std::string &ok, const float &a, const int &b)
{
	CPacket packet;
	packet << GetId();
	packet << 204;
	packet << ok;
	packet << a;
	packet << b;
	GetNetConnector()->Send(targetId, packet);
}



//------------------------------------------------------------------------
// Protocol: func2
//------------------------------------------------------------------------
void c2s_Protocol::func2(netid targetId, const std::string &str)
{
	CPacket packet;
	packet << GetId();
	packet << 300;
	packet << str;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: func3
//------------------------------------------------------------------------
void c2s_Protocol::func3(netid targetId, const float &value)
{
	CPacket packet;
	packet << GetId();
	packet << 301;
	packet << value;
	GetNetConnector()->Send(targetId, packet);
}



