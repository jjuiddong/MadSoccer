#include "stdafx.h"
#include "c2s_Protocol.h"
using namespace network;

//------------------------------------------------------------------------
// // 패킷의 프로토콜에 따라 해당하는 리스너의 함수를 호출한다.
//------------------------------------------------------------------------
void c2s_ProtocolListener::Dispatch(CPacket &packet, const ProtocolListenerList &listeners)
{
	BOOST_FOREACH(ProtocolListenerPtr p, listeners)
	{
		IProtocolListener *ptmp = p;
		c2s_ProtocolListener *lstr = static_cast<c2s_ProtocolListener*>(ptmp);
		if (!lstr)
		{
			error::ErrorLog( "c2s_ProtocolListener::Dispatch Convert Error" );
			continue;
		}

		int protocolId, packetId;
		packet >> protocolId >> packetId;
		switch (packetId)
		{
		case 300:
			{
				std::string str;
				packet >> str;
				lstr->func2(packet.GetSenderId(), str);
			}
			break;

		case 301:
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


//------------------------------------------------------------------------
// Protocol: func2
//------------------------------------------------------------------------
void c2s_Protocol::func2(netid targetId, const std::string &str)
{
	CPacket packet;
	packet << GetId();
	packet << 300;
	packet << str;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: func3
//------------------------------------------------------------------------
void c2s_Protocol::func3(netid targetId, const float &value)
{
	CPacket packet;
	packet << GetId();
	packet << 301;
	packet << value;
	GetNetConnector()->Send(targetId, packet);
}

