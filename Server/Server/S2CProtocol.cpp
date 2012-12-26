
#include "stdafx.h"
#include "S2CProtocol.h"
#include "C2SProtocolListener.h"

using namespace network;

//------------------------------------------------------------------------
// 패킷의 프로토콜에 따라 해당하는 리스너의 함수를 호출한다.
//------------------------------------------------------------------------
void CS2CProtocol::Dispatch(CPacket &packet, const ListenerList &listeners)
{
	BOOST_FOREACH(IProtocolListener *p, listeners)
	{
		CC2SProtocolListener *lstr = static_cast<CC2SProtocolListener*>(p);
		if (!lstr)
		{
			error::ErrorLog( "CS2CPacketDispatcher::Dispatch Conver Error" );
			continue;
		}

		int protocol;
		packet >> protocol;
		switch (protocol)
		{
		case 1:
			lstr->func1(packet.GetSenderId());
			break;

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
				lstr->func3(packet.GetSenderId(), value );
			}
			break;

		case 4:
			lstr->func4(packet.GetSenderId());
			break;

		default:
			assert(0);
			break;
		}
	}

}

//------------------------------------------------------------------------
// Protocol
//------------------------------------------------------------------------
void CS2CProtocol::func1(netid targetId)
{
	CPacket packet;
	packet << 1;
	GetNetObject()->Send(targetId, packet);
}
void CS2CProtocol::func2(netid targetId, const std::string &str)
{
	CPacket packet;
	packet << 2;
	packet << str;
	GetNetObject()->Send(targetId, packet);
}
void CS2CProtocol::func3(netid targetId, float value)
{
	CPacket packet;
	packet << 3;
	packet << value;
	GetNetObject()->Send(targetId, packet);
}
void CS2CProtocol::func4(netid targetId)
{
	CPacket packet;
	packet << 4;
	GetNetObject()->Send(targetId, packet);
}
