//------------------------------------------------------------------------
// Name:    S2CPacketDispatcher.h
// Author:  jjuiddong
// Date:    12/23/2012
// 
// 
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class CS2CProtocol : public IProtocol
	{
	public:
		virtual void Dispatch(const CPacket &packet, const ListenerList &listeners) override;
	};

}
