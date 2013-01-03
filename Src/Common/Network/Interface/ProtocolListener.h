//------------------------------------------------------------------------
// Name:    ProtocolListener.h
// Author:  jjuiddong
// Date:    12/26/2012
// 
// ���������� �޾� ó���ϴ� ������ �Ѵ�.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class IProtocolListener
	{
	public:
		IProtocolListener() {}
		virtual ~IProtocolListener() {}
	public:
		virtual void BeforePacketProcess(const CPacket &packet) {} // ��Ŷó���� �����ϱ� ���� ȣ��
		virtual void AfterPacketProcess(const CPacket &packet) {} // ��Ŷ�� ó���� �� ȣ��ȴ�.
	};
}
