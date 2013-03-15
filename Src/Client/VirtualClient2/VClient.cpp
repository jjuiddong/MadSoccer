
#include "stdafx.h"
#include "VClient.h"
#include "VirtualClient2.h"
#include "DlgConsole.h"
#include "ProtocolTree.h"

using namespace network;

CVClient::CVClient() : CClient(network::USER_LOOP)
{
	RegisterProtocol(&m_Protocol);
	AddListener(this);
	SetEventListener(this);

}

CVClient::~CVClient()
{

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CVClient::OnClientConnect(ClientPtr client)
{
	GetConsole()->AddString( "Connect" );
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CVClient::OnClientDisconnect(ClientPtr client)
{
	GetConsole()->AddString( "DisConnect" );
}


//------------------------------------------------------------------------
// 서버로부터 받은 모든 패킷을 추가한다.
//------------------------------------------------------------------------
void CVClient::recv(netid senderId, network::CPacket &packet)
{
	const int protocolID = packet.GetProtocolId();
	const int packetID = packet.GetPacketId();
	sProtocol *protocol = GetTree()->GetProtocol(protocolID, packetID);

	std::stringstream ss;
	ss << "Recv = ";
	ss << network::Packet2String(packet, protocol);

	GetConsole()->AddString( ss.str() );
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CVClient::AckGroupList(netid senderId, const int &errorCode, const GroupVector &groups)
{
	GetConsole()->AddString( "------------" );
	GetConsole()->AddString( 
		common::format( "group count: %d, errorCode: %d",
		groups.size(), errorCode) );		

	for (u_int i=0; i < groups.size(); ++i)
	{
		GetConsole()->AddString( "Group Information" );

		GetConsole()->AddString( 
			common::format( "    group id: %d",
				groups[ i].GetId()) );		

		GetConsole()->AddString( 
			common::format( "    group name: %s",
			groups[ i].GetName().c_str()) );		

		GetConsole()->AddString( 
			common::format( "    users: %d",
			groups[ i].GetUsers().size()) );
	}
}
