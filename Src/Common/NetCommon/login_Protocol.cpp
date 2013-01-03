#include "login_Protocol.h"
using namespace network;
using namespace login;

//------------------------------------------------------------------------
// Protocol: AckLogin
//------------------------------------------------------------------------
void login::s2c_Protocol::AckLogin(netid targetId, const std::string &id, const int &result)
{
	CPacket packet;
	packet << GetId();
	packet << 100;
	packet << id;
	packet << result;
	GetNetConnector()->Send(targetId, packet);
}



//------------------------------------------------------------------------
// Protocol: ReqLogin
//------------------------------------------------------------------------
void login::c2s_Protocol::ReqLogin(netid targetId, const std::string &id, const std::string &password)
{
	CPacket packet;
	packet << GetId();
	packet << 200;
	packet << id;
	packet << password;
	GetNetConnector()->Send(targetId, packet);
}



