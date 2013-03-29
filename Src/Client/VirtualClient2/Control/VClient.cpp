
#include "stdafx.h"
#include "VClient.h"
#include "../VirtualClient2.h"
#include "../UI/DlgLog.h"
#include "../UI/ProtocolTree.h"

#include "NetProtocol/Src/basic_ProtocolListener.cpp"
#include "NetProtocol/Src/basic_Protocol.cpp"
#include "NetProtocol/Src/p2pComm_ProtocolListener.cpp"
#include "NetProtocol/Src/p2pComm_Protocol.cpp"

using namespace network;

CVClient::CVClient() : CClient(network::USER_LOOP)
{
	RegisterProtocol(&m_Protocol);
	RegisterProtocol(&m_P2pProtocol);
	AddProtocolListener(this);
	SetEventListener(this);

}

CVClient::~CVClient()
{

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CVClient::OnClientConnect(ClientBasicPtr client)
{
	GetConsole()->AddString( "Connect" );
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CVClient::OnClientDisconnect(ClientBasicPtr client)
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
void CVClient::AckGroupList(netid senderId, const network::error::ERROR_CODE &errorCode, const GroupVector &groups)
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


/**
 @brief 
 */
void	CVClient::OnP2PConnect(ClientBasicPtr client)
{
	GetConsole()->AddString( "P2P Connect" );
}


/**
 @brief 
 */
void	CVClient::OnP2PDisconnect(ClientBasicPtr client)
{
	GetConsole()->AddString( "P2P Disconnect" );
}


/**
 @brief 
 */
void	CVClient::OnMemberJoin(ClientBasicPtr client, netid clientId)
{
	GetConsole()->AddString( "Member Join" );
}


/**
 @brief 
 */
void	CVClient::OnMemberLeave(ClientBasicPtr client, netid clientId)
{
	GetConsole()->AddString( "Member Leave" );
}


/**
 @brief 
 */
void CVClient::SendData(netid senderId)
{
	// Host 일때, 나머지 클라이언트들에게 패킷을 보낸다.
	if (IsP2PHostClient())
	{
		m_P2pProtocol.SendData( P2P_NETID, SEND_T );
	}
}
