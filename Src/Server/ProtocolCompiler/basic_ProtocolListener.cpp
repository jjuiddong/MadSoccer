#include "stdafx.h"
#include "basic_ProtocolListener.h"
using namespace network;
using namespace basic;

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


