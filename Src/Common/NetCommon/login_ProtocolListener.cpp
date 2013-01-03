#include "login_ProtocolListener.h"
#include "Network/Controller/NetController.h"
using namespace network;
using namespace login;


static login::s2c_Dispatcher g_login_s2c_Dispatcher;

login::s2c_Dispatcher::s2c_Dispatcher() 
	: IProtocolDispatcher(login::s2c_ProtocolListener_ID)
{
	CNetController::Get()->AddDispatcher(this);
}

//------------------------------------------------------------------------
// 해당되는 프로토콜 리스너를 찾아 함수를 호출한다.
//------------------------------------------------------------------------
void login::s2c_Dispatcher::Dispatch(CPacket &packet, const ProtocolListenerList &listeners)
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
		case 100:
			{
				std::string id;
				packet >> id;
				int result;
				packet >> result;
				lstr->AckLogin(packet.GetSenderId(), id, result);
			}
			break;

		default:
			assert(0);
			break;
		}
	}
}



static login::c2s_Dispatcher g_login_c2s_Dispatcher;

login::c2s_Dispatcher::c2s_Dispatcher() 
: IProtocolDispatcher(login::c2s_ProtocolListener_ID)
{
	CNetController::Get()->AddDispatcher(this);
}


//------------------------------------------------------------------------
// 해당되는 프로토콜 리스너를 찾아 함수를 호출한다.
//------------------------------------------------------------------------
void login::c2s_Dispatcher::Dispatch(CPacket &packet, const ProtocolListenerList &listeners)
{
	BOOST_FOREACH(ProtocolListenerPtr p, listeners)
	{
		IProtocolListener *ptmp = p;
		c2s_ProtocolListener *lstr = dynamic_cast<c2s_ProtocolListener*>(ptmp);
		if (!lstr)
		{
			//error::ErrorLog( "c2s_ProtocolListener::Dispatch Convert Error" );
			continue;
		}

		int protocolId, packetId;
		packet >> protocolId >> packetId;
		switch (packetId)
		{
		case 200:
			{
				std::string id;
				packet >> id;
				std::string password;
				packet >> password;
				lstr->ReqLogin(packet.GetSenderId(), id, password);
			}
			break;

		default:
			assert(0);
			break;
		}
	}
}

