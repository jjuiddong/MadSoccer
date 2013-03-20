#include "login_Protocol.h"
using namespace network;
using namespace marshalling;
using namespace login;

//------------------------------------------------------------------------
// Protocol: AckLogIn
//------------------------------------------------------------------------
void login::s2c_Protocol::AckLogIn(netid targetId, const network::SEND_FLAG flag, const std::string &id, const int &result)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 101 );
	packet << id;
	packet << result;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}

//------------------------------------------------------------------------
// Protocol: AckLogOut
//------------------------------------------------------------------------
void login::s2c_Protocol::AckLogOut(netid targetId, const network::SEND_FLAG flag, const std::string &id, const int &result)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 102 );
	packet << id;
	packet << result;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}



//------------------------------------------------------------------------
// Protocol: ReqLogIn
//------------------------------------------------------------------------
void login::c2s_Protocol::ReqLogIn(netid targetId, const network::SEND_FLAG flag, const std::string &id, const std::string &password)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 201 );
	packet << id;
	packet << password;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}

//------------------------------------------------------------------------
// Protocol: ReqLogOut
//------------------------------------------------------------------------
void login::c2s_Protocol::ReqLogOut(netid targetId, const network::SEND_FLAG flag, const std::string &id)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 202 );
	packet << id;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}



