
#include "stdafx.h"
#include "VClient.h"
#include "VirtualClient2.h"
#include "DlgConsole.h"
#include "ProtocolTree.h"

using namespace network;

CVClient::CVClient()
{
	RegisterProtocol(&m_Protocol);
	AddListener(this);

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CVClient::OnConnect()
{
	GetConsole()->AddString( "Connect" );
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CVClient::OnDisconnect()
{
	GetConsole()->AddString( "DisConnect" );
}


//------------------------------------------------------------------------
// �����κ��� ���� ��� ��Ŷ�� �߰��Ѵ�.
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
