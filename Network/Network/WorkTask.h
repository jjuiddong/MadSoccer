//------------------------------------------------------------------------
// Name:    WorkTask.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// 서버에 저장된 패킷을 실행시키는 워크쓰레드다.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class CWorkTask : public common::CTask
	{
	public:
		CWorkTask(CServer*psvr) : CTask(2), m_pServer(psvr) {}
		virtual ~CWorkTask() {}

	protected:
		common::ReferencePtr<CServer>	m_pServer;

	public:
		virtual RUN_RESULT	Run() override
		{
			if (!m_pServer) return RR_CONTINUE;

			m_pServer->EnterSync();
			const PacketList &packets = m_pServer->GetPackets();
			if (!packets.empty())
			{
				PacketCItor it = packets.begin();
				while (packets.end() != it)
				{
					m_pServer->ProcessPacket( *it++ );
				}
				m_pServer->ClearPackets();
			}
			m_pServer->LeaveSync();

			return RR_CONTINUE;
		}
	};

}
