#include "basic_ProtocolListener.h"
#include "Network/Controller/NetController.h"
#include "network/PrtCompiler/ProtocolMacro.h"

using namespace network;
using namespace basic;

static basic::s2c_Dispatcher g_basic_s2c_Dispatcher;

basic::s2c_Dispatcher::s2c_Dispatcher()
	: IProtocolDispatcher(basic::s2c_Dispatcher_ID)
{
	CNetController::Get()->AddDispatcher(this);
}

//------------------------------------------------------------------------
// 패킷의 프로토콜에 따라 해당하는 리스너의 함수를 호출한다.
//------------------------------------------------------------------------
void basic::s2c_Dispatcher::Dispatch(CPacket &packet, const ProtocolListenerList &listeners)
{
	int protocolId, packetId;
	packet >> protocolId >> packetId;
	switch (packetId)
	{
	case 501:
		{
			int result;
			packet >> result;
			network::P2P_STATE state;
			packet >> state;
			std::string ip;
			packet >> ip;
			int port;
			packet >> port;
			SEND_LISTENER(s2c_ProtocolListener, listeners, AckP2PConnect(packet.GetSenderId(), result, state, ip, port) );
		}
		break;

	case 502:
		{
			SEND_LISTENER(s2c_ProtocolListener, listeners, func1(packet.GetSenderId()) );
		}
		break;

	case 503:
		{
			std::string str;
			packet >> str;
			SEND_LISTENER(s2c_ProtocolListener, listeners, func2(packet.GetSenderId(), str) );
		}
		break;

	case 504:
		{
			float value;
			packet >> value;
			SEND_LISTENER(s2c_ProtocolListener, listeners, func3(packet.GetSenderId(), value) );
		}
		break;

	case 505:
		{
			SEND_LISTENER(s2c_ProtocolListener, listeners, func4(packet.GetSenderId()) );
		}
		break;

	case 506:
		{
			std::string ok;
			packet >> ok;
			float a;
			packet >> a;
			int b;
			packet >> b;
			SEND_LISTENER(s2c_ProtocolListener, listeners, func5(packet.GetSenderId(), ok, a, b) );
		}
		break;

	default:
		assert(0);
		break;
	}
}



static basic::c2s_Dispatcher g_basic_c2s_Dispatcher;

basic::c2s_Dispatcher::c2s_Dispatcher()
	: IProtocolDispatcher(basic::c2s_Dispatcher_ID)
{
	CNetController::Get()->AddDispatcher(this);
}

//------------------------------------------------------------------------
// 패킷의 프로토콜에 따라 해당하는 리스너의 함수를 호출한다.
//------------------------------------------------------------------------
void basic::c2s_Dispatcher::Dispatch(CPacket &packet, const ProtocolListenerList &listeners)
{
	int protocolId, packetId;
	packet >> protocolId >> packetId;
	switch (packetId)
	{
	case 601:
		{
			SEND_LISTENER(c2s_ProtocolListener, listeners, ReqP2PConnect(packet.GetSenderId()) );
		}
		break;

	case 602:
		{
			std::string str;
			packet >> str;
			SEND_LISTENER(c2s_ProtocolListener, listeners, func2(packet.GetSenderId(), str) );
		}
		break;

	case 603:
		{
			float value;
			packet >> value;
			SEND_LISTENER(c2s_ProtocolListener, listeners, func3(packet.GetSenderId(), value) );
		}
		break;

	default:
		assert(0);
		break;
	}
}



