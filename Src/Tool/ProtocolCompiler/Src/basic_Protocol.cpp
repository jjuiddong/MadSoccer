#include "basic_Protocol.h"
using namespace basic;

//------------------------------------------------------------------------
// Protocol: func1
//------------------------------------------------------------------------
void basic::s2c_Protocol::func1(netid targetId, const SEND_FLAG flag)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 201 );
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}

//------------------------------------------------------------------------
// Protocol: func2
//------------------------------------------------------------------------
void basic::s2c_Protocol::func2(netid targetId, const SEND_FLAG flag, const std::string &str)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 202 );
	packet << str;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}

//------------------------------------------------------------------------
// Protocol: func3
//------------------------------------------------------------------------
void basic::s2c_Protocol::func3(netid targetId, const SEND_FLAG flag, const float &value)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 203 );
	packet << value;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}

//------------------------------------------------------------------------
// Protocol: func4
//------------------------------------------------------------------------
void basic::s2c_Protocol::func4(netid targetId, const SEND_FLAG flag)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 204 );
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}



//------------------------------------------------------------------------
// Protocol: func2
//------------------------------------------------------------------------
void basic::c2s_Protocol::func2(netid targetId, const SEND_FLAG flag, const std::string &str)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 301 );
	packet << str;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}

//------------------------------------------------------------------------
// Protocol: func3
//------------------------------------------------------------------------
void basic::c2s_Protocol::func3(netid targetId, const SEND_FLAG flag, const float &value)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 302 );
	packet << value;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}



