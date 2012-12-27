
#include "stdafx.h"
#include "S2CProtocol.h"

using namespace network;

//------------------------------------------------------------------------
// Protocol
//------------------------------------------------------------------------
void S2CProtocol::func1(netid targetId)
{
	CPacket packet;
	packet << GetId();
	packet << 1;
	GetNetConnector()->Send(targetId, packet);
}
void S2CProtocol::func2(netid targetId, const std::string &str)
{
	CPacket packet;
	packet << GetId();
	packet << 2;
	packet << str;
	GetNetConnector()->Send(targetId, packet);
}
void S2CProtocol::func3(netid targetId, float value)
{
	CPacket packet;
	packet << GetId();
	packet << 3;
	packet << value;
	GetNetConnector()->Send(targetId, packet);
}
void S2CProtocol::func4(netid targetId)
{
	CPacket packet;
	packet << GetId();
	packet << 4;
	GetNetConnector()->Send(targetId, packet);
}


//------------------------------------------------------------------------
// 패킷의 프로토콜에 따라 해당하는 리스너의 함수를 호출한다.
//------------------------------------------------------------------------
void S2CProtocolListener::Dispatch(CPacket &packet, const ProtocolListenerList &listeners)
{
	BOOST_FOREACH(ProtocolListenerPtr p, listeners)
	{
		IProtocolListener *ptmp = p;
		S2CProtocolListener *lstr = static_cast<S2CProtocolListener*>(ptmp);
		if (!lstr)
		{
			error::ErrorLog( "S2CProtocolListener::Dispatch Conver Error" );
			continue;
		}

		int protocolId, packetId;
		packet >> protocolId >> packetId;
		switch (packetId)
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

