//------------------------------------------------------------------------
// Name:    PacketListener.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// ��Ŷ�� �޾Ƽ� ó���ϴ� Ŭ������ �� �������̽��� ��ӹ޾� �����ؾ� �Ѵ�.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class IPacketListener
	{
	public:
		virtual ~IPacketListener() {}

		virtual void BeforePacketProcess(const CPacket &packet) {} // ��Ŷó���� �����ϱ� ���� ȣ��
		virtual void AfterPacketProcess(const CPacket &packet) {} // ��Ŷ�� ó���� �� ȣ��ȴ�.
	};
}
