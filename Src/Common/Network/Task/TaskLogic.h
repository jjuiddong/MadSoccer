//------------------------------------------------------------------------
// Name:    TaskLogic.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// CPacketQueue�� ����� ��Ŷ�� ������ �Ľ��ؼ� Listener�� 
// ��ϵ� ��ü���� �޼����� ������.
//
// ���� �������� �޴� ��Ŷ�� ó���ϰ� �ִ�. Ŭ���̾�Ʈ�� �ڵ�� ���� ����.
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
				common::format("CTaskLogic:: netid: %d �� �ش��ϴ� ������ �����ϴ�.", 
				packetData.rcvNetId) );
			return RR_CONTINUE;
		}

		const ProtocolListenerList &listeners = pSvr->GetProtocolListeners();
		if (listeners.empty())
		{
			clog::Error( clog::ERROR_CRITICAL,
				common::format("CTaskLogic %d NetConnector�� �������� �����ʰ� �����ϴ�.", 
				pSvr->GetNetId()) );
			return RR_CONTINUE;
		}

		// ��� ��Ŷ�� �޾Ƽ� ó���ϴ� �����ʿ��� ��Ŷ�� ������.
		all::Dispatcher allDispatcher;
		allDispatcher.Dispatch(packetData.packet, listeners);
		// 

		const int protocolId = packetData.packet.GetProtocolId();

		// �⺻ �������� ó��
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
				common::format("CTaskLogic:: %d �� �ش��ϴ� �������� �����İ� �����ϴ�.", 
				protocolId) );
			return RR_CONTINUE;
		}

		pDispatcher->Dispatch(packetData.packet, listeners);

		return RR_CONTINUE;
	}
}
