#include "basic_Protocol.h"
using namespace network;
using namespace basic;

//------------------------------------------------------------------------
// Protocol: func1
//------------------------------------------------------------------------
void basic::s2c_Protocol::func1(netid targetId)
{
	CPacket packet;
	packet << GetId();
	packet << 201;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: func2
//------------------------------------------------------------------------
void basic::s2c_Protocol::func2(netid targetId, const std::string &str)
{
	CPacket packet;
	packet << GetId();
	packet << 202;
	packet << str;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: func3
//------------------------------------------------------------------------
void basic::s2c_Protocol::func3(netid targetId, const float &value)
{
	CPacket packet;
	packet << GetId();
	packet << 203;
	packet << value;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: func4
//------------------------------------------------------------------------
void basic::s2c_Protocol::func4(netid targetId)
{
	CPacket packet;
	packet << GetId();
	packet << 204;
	GetNetConnector()->Send(targetId, packet);
}



//------------------------------------------------------------------------
// Protocol: func2
//------------------------------------------------------------------------
void basic::c2s_Protocol::func2(netid targetId, const std::string &str)
{
	CPacket packet;
	packet << GetId();
	packet << 301;
	packet << str;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: func3
//------------------------------------------------------------------------
void basic::c2s_Protocol::func3(netid targetId, const float &value)
{
	CPacket packet;
	packet << GetId();
	packet << 302;
	packet << value;
	GetNetConnector()->Send(targetId, packet);
}



