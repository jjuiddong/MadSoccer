
#include "stdafx.h"
#include "C2SProtocol.h"
#include "S2CProtocolListener.h"

using namespace network;

//------------------------------------------------------------------------
// 패킷의 프로토콜에 따라 해당하는 리스너의 함수를 호출한다.
//------------------------------------------------------------------------
void CC2SProtocol::Dispatch(CPacket &packet, const ListenerList &listeners)
{
	BOOST_FOREACH(IProtocolListener *p, listeners)
	{
		CS2CProtocolListener *lstr = static_cast<CS2CProtocolListener*>(p);
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
void CC2SProtocol::func(netid targetId, const std::string &str)
{
	CPacket packet;
	packet << 2;
	packet << str;
	GetNetObject()->Send(targetId, packet);
}
