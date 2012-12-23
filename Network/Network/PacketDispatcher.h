//------------------------------------------------------------------------
// Name:    PacketDispatcher.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// 수신된 패킷을 해당하는 리스너에게 전달해준다.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class IPacketDispatcher
	{
	public:
 		virtual ~IPacketDispatcher() {}
	public:
		virtual void Dispatch(const CPacket &packet, const ListenerList &listeners)=0;
	};
}
