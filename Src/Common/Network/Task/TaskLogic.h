//------------------------------------------------------------------------
// Name:    TaskLogic.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// 네트워크로부터 받은 패킷을 파싱해서 Listener에 등록된 객체에게 메세지를
// 보낸다.
//------------------------------------------------------------------------
#pragma once

#include "../DataStructure/PacketQueue.h"
#include "../Controller/NetController.h"

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
					common::format("CTaskLogic:: %d 에 해당하는 서버가 없습니다.", 
						packetData.rcvServerSock) );
				return RR_CONTINUE;
			}

			const int protocolId = packetData.packet.GetProtocolId();
			IProtocolDispatcher *pDispatcher = CNetController::Get()->GetDispatcher(protocolId);
			if (!pDispatcher)
			{
				error::ErrorLog( 
					common::format("CTaskLogic:: %d 에 해당하는 프로토콜 디스패쳐가 없습니다.", 
					protocolId) );
				return RR_CONTINUE;
			}

 			const ProtocolListenerList &listeners = pSvr->GetListeners();
			if (listeners.empty())
			{
				error::ErrorLog( 
					common::format("CTaskLogic %d NetConnector의 프로토콜 리스너가 없습니다.", 
					pSvr->GetNetId() ) );
				return RR_CONTINUE;
			}
			else
			{
				//listeners.front()->Dispatch(packetData.packet, listeners);
				pDispatcher->Dispatch(packetData.packet, listeners);
			}

			return RR_CONTINUE;
		}
	};
}
