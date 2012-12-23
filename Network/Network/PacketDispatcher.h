//------------------------------------------------------------------------
// Name:    PacketDispatcher.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// ���ŵ� ��Ŷ�� �ش��ϴ� �����ʿ��� �������ش�.
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
