//------------------------------------------------------------------------
// Name:    TaskLogic.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// CPacketQueue에 저장된 패킷을 가져와 파싱해서 Listener에 
// 등록된 객체에게 메세지를 보낸다.
//------------------------------------------------------------------------
#pragma once

#include "../DataStructure/PacketQueue.h"
#include "../Controller/NetController.h"
#include "../Service/AllProtocolListener.h"


namespace network
{
	DECLARE_TYPE_NAME_SCOPE(network, CTaskLogic)
	class CTaskLogic : public common::CTask
		, public sharedmemory::CSharedMem<CTaskLogic, TYPE_NAME(network::CTaskLogic)>
	{
	public:
		CTaskLogic() : CTask(1,"TaskLogic") {}

	public:
		virtual RUN_RESULT	Run() override
		{
			CPacketQueue::SPacketData packetData;
			if (!CPacketQueue::Get()->PopPacket(packetData))
				return RR_CONTINUE;

			CServer *pSvr = GetServer(packetData.rcvNetId);
			if (!pSvr)
			{
				error::ErrorLog( 
					common::format("CTaskLogic:: netid: %d 에 해당하는 서버가 없습니다.", 
						packetData.rcvNetId) );
				return RR_CONTINUE;
			}

 			const ProtocolListenerList &listeners = pSvr->GetListeners();
			if (listeners.empty())
			{
				error::ErrorLog( 
					common::format("CTaskLogic %d NetConnector의 프로토콜 리스너가 없습니다.", 
					pSvr->GetNetId()) );
				return RR_CONTINUE;
			}

			// 모든 패킷을 받아서 처리하는 리스너에게 패킷을 보낸다.
			all::Dispatcher allDispatcher;
			allDispatcher.Dispatch(packetData.packet, listeners);
			// 

			const int protocolId = packetData.packet.GetProtocolId();
			IProtocolDispatcher *pDispatcher = CNetController::Get()->GetDispatcher(protocolId);
			if (!pDispatcher)
			{
				error::ErrorLog( 
					common::format("CTaskLogic:: %d 에 해당하는 프로토콜 디스패쳐가 없습니다.", 
					protocolId) );
				return RR_CONTINUE;
			}

			pDispatcher->Dispatch(packetData.packet, listeners);

			return RR_CONTINUE;
		}
	};
}
