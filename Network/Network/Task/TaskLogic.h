//------------------------------------------------------------------------
// Name:    TaskLogic.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// ��Ʈ��ũ�κ��� ���� ��Ŷ�� �Ľ��ؼ� Listener�� ��ϵ� ��ü���� �޼�����
// ������.
//------------------------------------------------------------------------
#pragma once

#include "PacketQueue.h"


namespace network
{
	class CTaskLogic : public common::CTask
	{
	public:
		CTaskLogic() : CTask(0) {}

	public:
		virtual RUN_RESULT	Run() override
		{
			CPacketQueue::SPacketData packetData;
			if (!CPacketQueue::Get()->PopPacket(packetData))
				return RR_CONTINUE;

			CServer *pSvr = GetServer(packetData.rcvServerSock);
			if (!pSvr)
			{
				error::ErrorLog( 
					common::format("CTaskLogic:: %d �� �ش��ϴ� ������ �����ϴ�.", 
						packetData.rcvServerSock) );
				return RR_CONTINUE;
			}

			ProtocolPtr protocol = pSvr->GetProtocol();
			if (!protocol)
			{
				error::ErrorLog( "Dispatcher�� �������� �ʾҽ��ϴ�." );
				return RR_CONTINUE;
			}

			// ��Ŷ�� ��ġ�ϴ� �������̽��� ȣ���Ѵ�.
			protocol->Dispatch(packetData.packet, pSvr->GetListeners() );

			return RR_CONTINUE;
		}
	};

}
