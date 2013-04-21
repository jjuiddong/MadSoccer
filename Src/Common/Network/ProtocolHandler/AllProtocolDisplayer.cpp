
#include "stdafx.h"
#include "AllProtocolDisplayer.h"
#include "../Utility/Protocols.h"

using namespace network;

AllProtocolDisplayer::AllProtocolDisplayer(int displayType) : 
	m_DisplayType(displayType)
{
}


/**
 @brief recv
 */
void AllProtocolDisplayer::recv(netid senderId, network::CPacket &packet)
{
	const int protocolID = packet.GetProtocolId();
	const int packetID = packet.GetPacketId();
	sProtocol *protocol = protocols::GetProtocol(packetID);
	std::stringstream ss;
	ss << "Recv = ";
	ss << network::Packet2String(packet, protocol);
	clog::Log( clog::LOG_OUTPUTWINDOW, ss.str() );
	clog::Log( clog::LOG_FILE, ss.str() );
}
