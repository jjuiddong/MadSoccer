
#include "stdafx.h"
#include "AllProtocolDisplayer.h"

using namespace network;

CAllProtocolDisplayer::CAllProtocolDisplayer(int displayType) : 
	m_DisplayType(displayType)
{
}


/**
 @brief recv
 */
void CAllProtocolDisplayer::recv(netid senderId, network::CPacket &packet)
{
	const int protocolID = packet.GetProtocolId();
	const int packetID = packet.GetPacketId();
	//sProtocol *protocol = GetTree()->GetProtocol(protocolID, packetID);
	//std::stringstream ss;
	//ss << "Recv = ";
	//ss << network::Packet2String(packet, protocol);
}
