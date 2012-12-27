#include "stdafx.h"
#include "s2c_Protocol.h"
using namespace network;

//------------------------------------------------------------------------
// // 패킷의 프로토콜에 따라 해당하는 리스너의 함수를 호출한다.
//------------------------------------------------------------------------
void s2c_ProtocolListener::Dispatch(CPacket &packet, const ProtocolListenerList &listeners)
{
	BOOST_FOREACH(ProtocolListenerPtr p, listeners)
	{
		IProtocolListener *ptmp = p;
		s2c_ProtocolListener *lstr = static_cast<s2c_ProtocolListener*>(ptmp);
		if (!lstr)
		{
			error::ErrorLog( "s2c_ProtocolListener::Dispatch Convert Error" );
			continue;
		}

		int protocolId, packetId;
		packet >> protocolId >> packetId;
		switch (packetId)
		{
		case 200:
			{
				lstr->func1(packet.GetSenderId());
			}
			break;

		case 201:
			{
				std::string str;
				packet >> str;
				lstr->func2(packet.GetSenderId(), str);
			}
			break;

		case 202:
			{
				float value;
				packet >> value;
				lstr->func3(packet.GetSenderId(), value);
			}
			break;

		case 203:
			{
				lstr->func4(packet.GetSenderId());
			}
			break;

		default:
			assert(0);
			break;
		}
	}
}


//------------------------------------------------------------------------
// Protocol: func1
//------------------------------------------------------------------------
void s2c_Protocol::func1(netid targetId)
{
	CPacket packet;
	packet << GetId();
	packet << 200;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: func2
//------------------------------------------------------------------------
void s2c_Protocol::func2(netid targetId, const std::string &str)
{
	CPacket packet;
	packet << GetId();
	packet << 201;
	packet << str;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: func3
//------------------------------------------------------------------------
void s2c_Protocol::func3(netid targetId, const float &value)
{
	CPacket packet;
	packet << GetId();
	packet << 202;
	packet << value;
	GetNetConnector()->Send(targetId, packet);
}

//------------------------------------------------------------------------
// Protocol: func4
//------------------------------------------------------------------------
void s2c_Protocol::func4(netid targetId)
{
	CPacket packet;
	packet << GetId();
	packet << 203;
	GetNetConnector()->Send(targetId, packet);
}

