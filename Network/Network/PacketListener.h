//------------------------------------------------------------------------
// Name:    PacketListener.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// 패킷을 받아서 처리하는 클래스는 이 인터페이스를 상속받아 구현해야 한다.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class IPacketListener
	{
	public:
		virtual ~IPacketListener() {}

		virtual void BeforePacketProcess(const CPacket &packet) {} // 패킷처리를 시작하기 전에 호출
		virtual void AfterPacketProcess(const CPacket &packet) {} // 패킷을 처리한 후 호출된다.
	};
}
