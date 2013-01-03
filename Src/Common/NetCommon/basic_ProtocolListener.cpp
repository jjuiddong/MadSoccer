#include "basic_ProtocolListener.h"
#include "Network/Controller/NetController.h"

using namespace network;
using namespace basic;

static basic::s2c_Dispatcher g_basic_s2c_Dispatcher;

basic::s2c_Dispatcher::s2c_Dispatcher()
	: IProtocolDispatcher(basic::s2c_Dispatcher_ID)
{
	CNetController::Get()->AddDispatcher(this);
}

//------------------------------------------------------------------------
// // 패킷의 프로토콜에 따라 해당하는 리스너의 함수를 호출한다.
//------------------------------------------------------------------------
void basic::s2c_Dispatcher::Dispatch(CPacket &packet, const ProtocolListenerList &listeners)
{
	BOOST_FOREACH(ProtocolListenerPtr p, listeners)
	{
		IProtocolListener *ptmp = p;
		s2c_ProtocolListener *lstr = dynamic_cast<s2c_ProtocolListener*>(ptmp);
		if (!lstr)
		{
			continue;
		}

		int protocolId, packetId;
		packet >> protocolId >> packetId;
		switch (packetId)
		{
		case 500:
			{
				lstr->func1(packet.GetSenderId());
			}
			break;

		case 501:
			{
				std::string str;
				packet >> str;
				lstr->func2(packet.GetSenderId(), str);
			}
			break;

		case 502:
			{
				float value;
				packet >> value;
				lstr->func3(packet.GetSenderId(), value);
			}
			break;

		case 503:
			{
				lstr->func4(packet.GetSenderId());
			}
			break;

		case 504:
			{
				std::string ok;
				packet >> ok;
				float a;
				packet >> a;
				int b;
				packet >> b;
				lstr->func5(packet.GetSenderId(), ok, a, b);
			}
			break;

		default:
			assert(0);
			break;
		}
	}
}



static basic::c2s_Dispatcher g_basic_c2s_Dispatcher;

basic::c2s_Dispatcher::c2s_Dispatcher()
	: IProtocolDispatcher(basic::c2s_Dispatcher_ID)
{
	CNetController::Get()->AddDispatcher(this);
}

//------------------------------------------------------------------------
// // 패킷의 프로토콜에 따라 해당하는 리스너의 함수를 호출한다.
//------------------------------------------------------------------------
void basic::c2s_Dispatcher::Dispatch(CPacket &packet, const ProtocolListenerList &listeners)
{
	BOOST_FOREACH(ProtocolListenerPtr p, listeners)
	{
		IProtocolListener *ptmp = p;
		c2s_ProtocolListener *lstr = dynamic_cast<c2s_ProtocolListener*>(ptmp);
		if (!lstr)
		{
			continue;
		}

		int protocolId, packetId;
		packet >> protocolId >> packetId;
		switch (packetId)
		{
		case 600:
			{
				std::string str;
				packet >> str;
				lstr->func2(packet.GetSenderId(), str);
			}
			break;

		case 601:
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



