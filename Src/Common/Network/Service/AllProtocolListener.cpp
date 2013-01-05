
#include "stdafx.h"
#include "AllProtocolListener.h"
#include "Network/Controller/NetController.h"
#include "network/PrtCompiler/ProtocolMacro.h"

using namespace network;
using namespace all;

//------------------------------------------------------------------------
// 패킷의 프로토콜에 따라 해당하는 리스너의 함수를 호출한다.
//------------------------------------------------------------------------
void all::Dispatcher::Dispatch(CPacket &packet, const ProtocolListenerList &listeners)
{
	CPacket newPacket = packet;
	SEND_LISTENER(ProtocolListener, listeners, recv(packet.GetSenderId(), newPacket) );
}
