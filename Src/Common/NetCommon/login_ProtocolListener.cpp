#include "login_ProtocolListener.h"
#include "Network/Controller/NetController.h"
#include "network/PrtCompiler/ProtocolMacro.h"

using namespace network;
using namespace login;

static login::s2c_Dispatcher g_login_s2c_Dispatcher;

login::s2c_Dispatcher::s2c_Dispatcher()
	: IProtocolDispatcher(login::s2c_Dispatcher_ID)
{
	CNetController::Get()->AddDispatcher(this);
}

//------------------------------------------------------------------------
// 패킷의 프로토콜에 따라 해당하는 리스너의 함수를 호출한다.
//------------------------------------------------------------------------
void login::s2c_Dispatcher::Dispatch(CPacket &packet, const ProtocolListenerList &listeners)
{
	int protocolId, packetId;
	packet >> protocolId >> packetId;
	switch (packetId)
	{
	case 101:
		{
			std::string id;
			packet >> id;
			int result;
			packet >> result;
			SEND_LISTENER(s2c_ProtocolListener, listeners, AckLogIn(packet.GetSenderId(), id, result) );
		}
		break;

	case 102:
		{
			std::string id;
			packet >> id;
			int result;
			packet >> result;
			SEND_LISTENER(s2c_ProtocolListener, listeners, AckLogOut(packet.GetSenderId(), id, result) );
		}
		break;

	default:
		assert(0);
		break;
	}
}



static login::c2s_Dispatcher g_login_c2s_Dispatcher;

login::c2s_Dispatcher::c2s_Dispatcher()
	: IProtocolDispatcher(login::c2s_Dispatcher_ID)
{
	CNetController::Get()->AddDispatcher(this);
}

//------------------------------------------------------------------------
// 패킷의 프로토콜에 따라 해당하는 리스너의 함수를 호출한다.
//------------------------------------------------------------------------
void login::c2s_Dispatcher::Dispatch(CPacket &packet, const ProtocolListenerList &listeners)
{
	int protocolId, packetId;
	packet >> protocolId >> packetId;
	switch (packetId)
	{
	case 201:
		{
			std::string id;
			packet >> id;
			std::string password;
			packet >> password;
			SEND_LISTENER(c2s_ProtocolListener, listeners, ReqLogIn(packet.GetSenderId(), id, password) );
		}
		break;

	case 202:
		{
			std::string id;
			packet >> id;
			SEND_LISTENER(c2s_ProtocolListener, listeners, ReqLogOut(packet.GetSenderId(), id) );
		}
		break;

	default:
		assert(0);
		break;
	}
}



