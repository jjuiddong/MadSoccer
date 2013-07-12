#include "basic_ProtocolListener.h"
#include "Network/Controller/Controller.h"

using namespace basic;

static basic::s2c_Dispatcher g_basic_s2c_Dispatcher;

basic::s2c_Dispatcher::s2c_Dispatcher()
	: IProtocolDispatcher(basic::s2c_Dispatcher_ID)
{
	CController::Get()->AddDispatcher(this);
}

//------------------------------------------------------------------------
// 패킷의 프로토콜에 따라 해당하는 리스너의 함수를 호출한다.
//------------------------------------------------------------------------
bool basic::s2c_Dispatcher::Dispatch(CPacket &packet, const ProtocolListenerList &listeners)
{
	const int protocolId = packet.GetProtocolId();
	const int packetId = packet.GetPacketId();
	switch (packetId)
	{
	case 201:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<s2c_ProtocolListener>(listeners, recvListener))
				return false;

			SetCurrentDispatchPacket( &packet );

			func1_Packet data;
			data.pdispatcher = this;
			data.senderId = packet.GetSenderId();
			SEND_LISTENER(s2c_ProtocolListener, recvListener, func1(data));
		}
		break;

	case 202:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<s2c_ProtocolListener>(listeners, recvListener))
				return false;

			SetCurrentDispatchPacket( &packet );

			func2_Packet data;
			data.pdispatcher = this;
			data.senderId = packet.GetSenderId();
			packet >> data.str;
			SEND_LISTENER(s2c_ProtocolListener, recvListener, func2(data));
		}
		break;

	case 203:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<s2c_ProtocolListener>(listeners, recvListener))
				return false;

			SetCurrentDispatchPacket( &packet );

			func3_Packet data;
			data.pdispatcher = this;
			data.senderId = packet.GetSenderId();
			packet >> data.value;
			SEND_LISTENER(s2c_ProtocolListener, recvListener, func3(data));
		}
		break;

	case 204:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<s2c_ProtocolListener>(listeners, recvListener))
				return false;

			SetCurrentDispatchPacket( &packet );

			func4_Packet data;
			data.pdispatcher = this;
			data.senderId = packet.GetSenderId();
			SEND_LISTENER(s2c_ProtocolListener, recvListener, func4(data));
		}
		break;

	default:
		assert(0);
		break;
	}
	return true;
}



static basic::c2s_Dispatcher g_basic_c2s_Dispatcher;

basic::c2s_Dispatcher::c2s_Dispatcher()
	: IProtocolDispatcher(basic::c2s_Dispatcher_ID)
{
	CController::Get()->AddDispatcher(this);
}

//------------------------------------------------------------------------
// 패킷의 프로토콜에 따라 해당하는 리스너의 함수를 호출한다.
//------------------------------------------------------------------------
bool basic::c2s_Dispatcher::Dispatch(CPacket &packet, const ProtocolListenerList &listeners)
{
	const int protocolId = packet.GetProtocolId();
	const int packetId = packet.GetPacketId();
	switch (packetId)
	{
	case 301:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<c2s_ProtocolListener>(listeners, recvListener))
				return false;

			SetCurrentDispatchPacket( &packet );

			func2_Packet data;
			data.pdispatcher = this;
			data.senderId = packet.GetSenderId();
			packet >> data.str;
			SEND_LISTENER(c2s_ProtocolListener, recvListener, func2(data));
		}
		break;

	case 302:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<c2s_ProtocolListener>(listeners, recvListener))
				return false;

			SetCurrentDispatchPacket( &packet );

			func3_Packet data;
			data.pdispatcher = this;
			data.senderId = packet.GetSenderId();
			packet >> data.value;
			SEND_LISTENER(c2s_ProtocolListener, recvListener, func3(data));
		}
		break;

	default:
		assert(0);
		break;
	}
	return true;
}



