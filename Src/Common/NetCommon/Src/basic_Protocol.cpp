#include "basic_Protocol.h"
using namespace network;
using namespace marshalling;
using namespace basic;

//------------------------------------------------------------------------
// Protocol: AckGroupList
//------------------------------------------------------------------------
void basic::s2c_Protocol::AckGroupList(netid targetId, const network::SEND_FLAG flag, const int &errorCode, const GroupVector &groups)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 501 );
	packet << errorCode;
	packet << groups;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}

//------------------------------------------------------------------------
// Protocol: AckGroupJoin
//------------------------------------------------------------------------
void basic::s2c_Protocol::AckGroupJoin(netid targetId, const network::SEND_FLAG flag, const int &errorCode)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 502 );
	packet << errorCode;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}

//------------------------------------------------------------------------
// Protocol: AckGroupCreate
//------------------------------------------------------------------------
void basic::s2c_Protocol::AckGroupCreate(netid targetId, const network::SEND_FLAG flag, const int &errorCode, const std::string &groupName, const netid &groupid)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 503 );
	packet << errorCode;
	packet << groupName;
	packet << groupid;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}

//------------------------------------------------------------------------
// Protocol: JoinMember
//------------------------------------------------------------------------
void basic::s2c_Protocol::JoinMember(netid targetId, const network::SEND_FLAG flag, const netid &toGroupId, const netid &fromGroupId, const netid &userId)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 504 );
	packet << toGroupId;
	packet << fromGroupId;
	packet << userId;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}

//------------------------------------------------------------------------
// Protocol: AckP2PConnect
//------------------------------------------------------------------------
void basic::s2c_Protocol::AckP2PConnect(netid targetId, const network::SEND_FLAG flag, const int &errorCode, const network::P2P_STATE &state, const std::string &ip, const int &port)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 505 );
	packet << errorCode;
	packet << state;
	packet << ip;
	packet << port;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}

//------------------------------------------------------------------------
// Protocol: func1
//------------------------------------------------------------------------
void basic::s2c_Protocol::func1(netid targetId, const network::SEND_FLAG flag)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 506 );
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}

//------------------------------------------------------------------------
// Protocol: func2
//------------------------------------------------------------------------
void basic::s2c_Protocol::func2(netid targetId, const network::SEND_FLAG flag, const std::string &str)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 507 );
	packet << str;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}

//------------------------------------------------------------------------
// Protocol: func3
//------------------------------------------------------------------------
void basic::s2c_Protocol::func3(netid targetId, const network::SEND_FLAG flag, const float &value)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 508 );
	packet << value;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}

//------------------------------------------------------------------------
// Protocol: func4
//------------------------------------------------------------------------
void basic::s2c_Protocol::func4(netid targetId, const network::SEND_FLAG flag)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 509 );
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}

//------------------------------------------------------------------------
// Protocol: func5
//------------------------------------------------------------------------
void basic::s2c_Protocol::func5(netid targetId, const network::SEND_FLAG flag, const std::string &ok, const float &a, const int &b)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 510 );
	packet << ok;
	packet << a;
	packet << b;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}



//------------------------------------------------------------------------
// Protocol: ReqGroupList
//------------------------------------------------------------------------
void basic::c2s_Protocol::ReqGroupList(netid targetId, const network::SEND_FLAG flag, const netid &groupid)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 601 );
	packet << groupid;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}

//------------------------------------------------------------------------
// Protocol: ReqGroupJoin
//------------------------------------------------------------------------
void basic::c2s_Protocol::ReqGroupJoin(netid targetId, const network::SEND_FLAG flag, const netid &groupid)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 602 );
	packet << groupid;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}

//------------------------------------------------------------------------
// Protocol: ReqGroupCreate
//------------------------------------------------------------------------
void basic::c2s_Protocol::ReqGroupCreate(netid targetId, const network::SEND_FLAG flag, const netid &parentGroupId, const std::string &groupName)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 603 );
	packet << parentGroupId;
	packet << groupName;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}

//------------------------------------------------------------------------
// Protocol: ReqP2PConnect
//------------------------------------------------------------------------
void basic::c2s_Protocol::ReqP2PConnect(netid targetId, const network::SEND_FLAG flag)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 604 );
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}

//------------------------------------------------------------------------
// Protocol: ReqP2PConnectTryResult
//------------------------------------------------------------------------
void basic::c2s_Protocol::ReqP2PConnectTryResult(netid targetId, const network::SEND_FLAG flag, const bool &isSuccess)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 605 );
	packet << isSuccess;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}

//------------------------------------------------------------------------
// Protocol: func2
//------------------------------------------------------------------------
void basic::c2s_Protocol::func2(netid targetId, const network::SEND_FLAG flag, const std::string &str)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 606 );
	packet << str;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}

//------------------------------------------------------------------------
// Protocol: func3
//------------------------------------------------------------------------
void basic::c2s_Protocol::func3(netid targetId, const network::SEND_FLAG flag, const float &value)
{
	CPacket packet;
	packet.SetProtocolId( GetId() );
	packet.SetPacketId( 607 );
	packet << value;
	packet.EndPack();
	GetNetConnector()->Send(targetId, flag, packet);
}



