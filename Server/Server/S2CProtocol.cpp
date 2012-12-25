
#include "stdafx.h"
#include "S2CProtocol.h"
#include "S2CPacketListener.h"

using namespace network;


//------------------------------------------------------------------------
// 패킷의 프로토콜에 따라 해당하는 리스너의 함수를 호출한다.
//------------------------------------------------------------------------
void CS2CProtocol::Dispatch(const CPacket &packet, const ListenerList &listeners)
{
	BOOST_FOREACH(IPacketListener *p, listeners)
	{
		CS2CPacketListener *lstr = static_cast<CS2CPacketListener*>(p);
		if (!lstr)
		{
			error::ErrorLog( "CS2CPacketDispatcher::Dispatch Conver Error" );
			continue;
		}

		switch (packet.GetProtocol())
		{
		case 1:
			lstr->func1();
			break;

		case 2:
			lstr->func2();
			break;

		case 3:
			lstr->func3();
			break;

		case 4:
			lstr->func4();
			break;

		default:
			assert(0);
			break;
		}
	}

}
