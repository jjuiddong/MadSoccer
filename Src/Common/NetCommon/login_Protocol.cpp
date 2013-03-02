#include "login_Protocol.h"
using namespace network;
using namespace login;

//------------------------------------------------------------------------
// Protocol: AckLogIn
//------------------------------------------------------------------------
void login::s2c_Protocol::AckLogIn(netid targetId, const std::string &id, const int &result)
{
	CPacket packet;
	packet << GetId();
	packet << 101;
	packet << id;
	packet << result;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: AckLogOut
//------------------------------------------------------------------------
void login::s2c_Protocol::AckLogOut(netid targetId, const std::string &id, const int &result)
{
	CPacket packet;
	packet << GetId();
	packet << 102;
	packet << id;
	packet << result;
	GetNetConnector()->Send(targetId, packet);
}



//------------------------------------------------------------------------
// Protocol: ReqLogIn
//------------------------------------------------------------------------
void login::c2s_Protocol::ReqLogIn(netid targetId, const std::string &id, const std::string &password)
{
	CPacket packet;
	packet << GetId();
	packet << 201;
	packet << id;
	packet << password;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: ReqLogOut
//------------------------------------------------------------------------
void login::c2s_Protocol::ReqLogOut(netid targetId, const std::string &id)
{
	CPacket packet;
	packet << GetId();
	packet << 202;
	packet << id;
	GetNetConnector()->Send(targetId, packet);
}



