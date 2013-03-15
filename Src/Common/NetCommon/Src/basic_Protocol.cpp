#include "basic_Protocol.h"
using namespace network;
using namespace marshalling;
using namespace basic;

//------------------------------------------------------------------------
// Protocol: AckGroupList
//------------------------------------------------------------------------
void basic::s2c_Protocol::AckGroupList(netid targetId, const int &errorCode, const GroupVector &groups)
{
	CPacket packet;
	packet << GetId();
	packet << 501;
	packet << errorCode;
	packet << groups;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: AckGroupJoin
//------------------------------------------------------------------------
void basic::s2c_Protocol::AckGroupJoin(netid targetId, const int &errorCode)
{
	CPacket packet;
	packet << GetId();
	packet << 502;
	packet << errorCode;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: AckGroupCreate
//------------------------------------------------------------------------
void basic::s2c_Protocol::AckGroupCreate(netid targetId, const int &errorCode, const std::string &groupName, const netid &groupid)
{
	CPacket packet;
	packet << GetId();
	packet << 503;
	packet << errorCode;
	packet << groupName;
	packet << groupid;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: AckP2PConnect
//------------------------------------------------------------------------
void basic::s2c_Protocol::AckP2PConnect(netid targetId, const int &errorCode, const network::P2P_STATE &state, const std::string &ip, const int &port)
{
	CPacket packet;
	packet << GetId();
	packet << 504;
	packet << errorCode;
	packet << state;
	packet << ip;
	packet << port;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: func1
//------------------------------------------------------------------------
void basic::s2c_Protocol::func1(netid targetId)
{
	CPacket packet;
	packet << GetId();
	packet << 505;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: func2
//------------------------------------------------------------------------
void basic::s2c_Protocol::func2(netid targetId, const std::string &str)
{
	CPacket packet;
	packet << GetId();
	packet << 506;
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
	packet << 507;
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
	packet << 508;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: func5
//------------------------------------------------------------------------
void basic::s2c_Protocol::func5(netid targetId, const std::string &ok, const float &a, const int &b)
{
	CPacket packet;
	packet << GetId();
	packet << 509;
	packet << ok;
	packet << a;
	packet << b;
	GetNetConnector()->Send(targetId, packet);
}



//------------------------------------------------------------------------
// Protocol: ReqGroupList
//------------------------------------------------------------------------
void basic::c2s_Protocol::ReqGroupList(netid targetId, const netid &groupid)
{
	CPacket packet;
	packet << GetId();
	packet << 601;
	packet << groupid;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: ReqGroupJoin
//------------------------------------------------------------------------
void basic::c2s_Protocol::ReqGroupJoin(netid targetId, const netid &groupid)
{
	CPacket packet;
	packet << GetId();
	packet << 602;
	packet << groupid;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: ReqGroupCreate
//------------------------------------------------------------------------
void basic::c2s_Protocol::ReqGroupCreate(netid targetId, const netid &parentGroupId, const std::string &groupName)
{
	CPacket packet;
	packet << GetId();
	packet << 603;
	packet << parentGroupId;
	packet << groupName;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: ReqP2PConnect
//------------------------------------------------------------------------
void basic::c2s_Protocol::ReqP2PConnect(netid targetId)
{
	CPacket packet;
	packet << GetId();
	packet << 604;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: func2
//------------------------------------------------------------------------
void basic::c2s_Protocol::func2(netid targetId, const std::string &str)
{
	CPacket packet;
	packet << GetId();
	packet << 605;
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
	packet << 606;
	packet << value;
	GetNetConnector()->Send(targetId, packet);
}



