
#include "stdafx.h"
#include "C2SProtocol.h"

using namespace network;

//------------------------------------------------------------------------
// Protocol
//------------------------------------------------------------------------
void C2SProtocol::func2(netid targetId, const std::string &str)
{
	CPacket packet;
	packet << GetId();
	packet << 2;
	packet << str;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protoco2
//------------------------------------------------------------------------
void C2SProtocol::func3(netid targetId, float value)
{
	CPacket packet;
	packet << GetId();
	packet << 3;
	packet << value;
	GetNetConnector()->Send(targetId, packet);
}



//------------------------------------------------------------------------
// 패킷의 프로토콜에 따라 해당하는 리스너의 함수를 호출한다.
//------------------------------------------------------------------------
void C2SProtocolListener::Dispatch(CPacket &packet, const ProtocolListenerList &listeners)
{
	BOOST_FOREACH(ProtocolListenerPtr p, listeners)
	{
		IProtocolListener *ptmp = p;
		C2SProtocolListener *lstr = static_cast<C2SProtocolListener*>(ptmp);
		if (!lstr)
		{
			error::ErrorLog( "C2SProtocolListener::Dispatch Convert Error" );
			continue;
		}

		int protocolId, packetId;
		packet >> protocolId >> packetId;
		switch (packetId)
		{
		case 2:
			{
				std::string str;
				packet >> str;
				lstr->func2(packet.GetSenderId(), str);
			}
			break;
		case 3:
			{
				float value;
				packet >> value;
				lstr->func3(packet.GetSenderId(), value);
			}
			break;

		default:
			assert(0);
			break;
		}
	}	
}

