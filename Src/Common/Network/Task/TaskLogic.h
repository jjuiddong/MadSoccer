//------------------------------------------------------------------------
// Name:    TaskLogic.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// ��Ʈ��ũ�κ��� ���� ��Ŷ�� �Ľ��ؼ� Listener�� ��ϵ� ��ü���� �޼�����
// ������.
//------------------------------------------------------------------------
#pragma once

#include "../DataStructure/PacketQueue.h"

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

			const int protocolId = packetData.packet.GetProtocolId();
			const ProtocolListenerList &listeners = pSvr->GetListeners( protocolId );
			if (listeners.empty())
			{
				error::ErrorLog( 
					common::format("CTaskLogic:: %d �� �ش��ϴ� �������� �����ʰ� �����ϴ�.", 
					protocolId) );
				return RR_CONTINUE;
			}
			else
			{
				listeners.front()->Dispatch(packetData.packet, listeners);
			}

			return RR_CONTINUE;
		}
	};
}
