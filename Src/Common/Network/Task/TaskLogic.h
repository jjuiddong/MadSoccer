//------------------------------------------------------------------------
// Name:    TaskLogic.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// CPacketQueue에 저장된 패킷을 가져와 파싱해서 Listener에 
// 등록된 객체에게 메세지를 보낸다.
//
// 현재 서버에서 받는 패킷만 처리하고 있다. 클라이언트용 코드는 아직 없다.
//------------------------------------------------------------------------
#pragma once

#include "../Controller/NetController.h"
#include "../Service/AllProtocolListener.h"
#include "../ProtocolHandler/BasicProtocolDispatcher.h"

namespace network
{
	DECLARE_TYPE_NAME_SCOPE(network, CTaskLogic)
	class CTaskLogic : public common::CTask
		, public memmonitor::Monitor<CTaskLogic, TYPE_NAME(network::CTaskLogic)>
	{
	public:
		CTaskLogic();
		virtual ~CTaskLogic();
		virtual RUN_RESULT	Run() override;
	};


	inline CTaskLogic::CTaskLogic() : 
		CTask(1,"TaskLogic") 
	{
	}

	inline CTaskLogic::~CTaskLogic()
	{
	}

	//------------------------------------------------------------------------
	// Run
	//------------------------------------------------------------------------
	inline common::CTask::RUN_RESULT CTaskLogic::Run()
	{
		CPacketQueue::SPacketData packetData;
		if (!CPacketQueue::Get()->PopPacket(packetData))
			return RR_CONTINUE;

		CServerBasic *pSvr = GetServer(packetData.rcvNetId);
		if (!pSvr)
		{
			clog::Error( clog::ERROR_PROBLEM,
				common::format("CTaskLogic:: netid: %d 에 해당하는 서버가 없습니다.", 
				packetData.rcvNetId) );
			return RR_CONTINUE;
		}

		const ProtocolListenerList &listeners = pSvr->GetProtocolListeners();
		if (listeners.empty())
		{
			clog::Error( clog::ERROR_CRITICAL,
				common::format("CTaskLogic %d NetConnector의 프로토콜 리스너가 없습니다.", 
				pSvr->GetNetId()) );
			return RR_CONTINUE;
		}

		// 모든 패킷을 받아서 처리하는 리스너에게 패킷을 보낸다.
		all::Dispatcher allDispatcher;
		allDispatcher.Dispatch(packetData.packet, listeners);
		// 

		const int protocolId = packetData.packet.GetProtocolId();

		// 기본 프로토콜 처리
		if (protocolId == 0)
		{
			basic_protocol::ServerDispatcher dispatcher;
			dispatcher.Dispatch( packetData.packet, pSvr );
			return RR_CONTINUE;
		}

		IProtocolDispatcher *pDispatcher = CNetController::Get()->GetDispatcher(protocolId);
		if (!pDispatcher)
		{
			clog::Error( clog::ERROR_WARNING,
				common::format("CTaskLogic:: %d 에 해당하는 프로토콜 디스패쳐가 없습니다.", 
				protocolId) );
			return RR_CONTINUE;
		}

		pDispatcher->Dispatch(packetData.packet, listeners);

		return RR_CONTINUE;
	}
}
