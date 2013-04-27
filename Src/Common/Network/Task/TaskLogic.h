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
		CNetConnector* GetReceiveConnector(netid netId, OUT CONNECTOR_TYPE &type);
		void UpdateSenderNetId(CNetConnector *pCon,  CONNECTOR_TYPE type, CPacket &packet);
	};


	inline CTaskLogic::CTaskLogic() : 
		CTask(1,"TaskLogic") 
	{
	}

	inline CTaskLogic::~CTaskLogic()
	{
	}


	/**
	 @brief 
	 */
	CNetConnector* CTaskLogic::GetReceiveConnector(netid netId, OUT CONNECTOR_TYPE &type)
	{
		CNetConnector *pCon = GetServer(netId);
		if (pCon) 
			type = CON_SERVER;
		if (!pCon) 
		{
			pCon = GetClient(netId);
			if (pCon) 
				type = CON_CLIENT;
		}
		if (!pCon) 
		{
			pCon = GetCoreClient(netId);
			if (pCon) type = CON_CORECLIENT;
		}
		return pCon;
	}


	/**
	 @brief Ŭ���̾�Ʈ���� ��Ŷ�� ������, packet �� Sender�� ���� Id �� �����Ѵ�.
	 */
	void CTaskLogic::UpdateSenderNetId(CNetConnector *pCon,  CONNECTOR_TYPE type, CPacket &packet)
	{
		switch (type)
		{
		case CON_SERVER: break; // nothing

		case CON_CORECLIENT:
			{
				CCoreClient *pClient = dynamic_cast<CCoreClient*>(pCon);
				if (pClient)
				{
					packet.SetSenderId(pClient->GetServerNetId());
				}				
			}
			break;
		case CON_CLIENT:
			{
				CClientBasic *pClient = dynamic_cast<CClientBasic*>(pCon);
				if (pClient && pClient->GetConnectSvrClient())
				{
					packet.SetSenderId( pClient->GetConnectSvrClient()->GetServerNetId() );
				}
			}
			break;
		}
	}


	/**
	 @brief Run
	 */
	inline common::CTask::RUN_RESULT CTaskLogic::Run()
	{
		// Main Timer
		CNetController::Get()->MainLoop();
		//

		CPacketQueue::SPacketData packetData;
		if (!CPacketQueue::Get()->PopPacket(packetData))
			return RR_CONTINUE;

		CONNECTOR_TYPE type;
		CNetConnector *pCon = GetReceiveConnector(packetData.rcvNetId, type);
		if (!pCon)
		{
			clog::Error( clog::ERROR_PROBLEM,
				common::format("CTaskLogic:: netid: %d �� �ش��ϴ� NetConnector�� �����ϴ�.\n", 
				packetData.rcvNetId) );
			return RR_CONTINUE;
		}

		UpdateSenderNetId(pCon, type, packetData.packet);

		const ProtocolListenerList &listeners = pCon->GetProtocolListeners();

		// ��� ��Ŷ�� �޾Ƽ� ó���ϴ� �����ʿ��� ��Ŷ�� ������.
		all::Dispatcher allDispatcher;
		allDispatcher.Dispatch(packetData.packet, listeners);
		// 

		const int protocolId = packetData.packet.GetProtocolId();

		// �⺻ �������� ó��
		if (protocolId == 0)
		{
			switch (type)
			{
			case CON_SERVER:
				{
					basic_protocol::ServerDispatcher dispatcher;
					dispatcher.Dispatch( packetData.packet, dynamic_cast<CServerBasic*>(pCon) );
				}
			case CON_CORECLIENT:
				{
					basic_protocol::ClientDispatcher dispatcher;
					dispatcher.Dispatch( packetData.packet, dynamic_cast<CCoreClient*>(pCon) );
				}
				break;
			}
			return RR_CONTINUE;
		}

		if (listeners.empty())
		{
			clog::Error( clog::ERROR_CRITICAL,
				common::format("CTaskLogic %d NetConnector�� �������� �����ʰ� �����ϴ�.\n", 
				pCon->GetNetId()) );
			return RR_CONTINUE;
		}

		IProtocolDispatcher *pDispatcher = CNetController::Get()->GetDispatcher(protocolId);
		if (!pDispatcher)
		{
			clog::Error( clog::ERROR_WARNING,
				common::format("CTaskLogic:: %d �� �ش��ϴ� �������� �����İ� �����ϴ�.\n", 
				protocolId) );
			return RR_CONTINUE;
		}

		pDispatcher->Dispatch(packetData.packet, listeners);

		return RR_CONTINUE;
	}
}
