//------------------------------------------------------------------------
// Name:    PacketQueue.h
// Author:  jjuiddong
// Date:    12/24/2012
// 
// 네트워크를 통해 받은 패킷을 저장한다. 모든 서버와 클라이언트는 하나의 
// 패킷큐에 저장된다.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class CPacketQueue : public common::CSingleton<CPacketQueue>
	{
	public:
		CPacketQueue() { InitializeCriticalSection(&m_CriticalSection); }
		virtual ~CPacketQueue() { Clear(); }
		friend class CTaskLogic;
		friend class CTaskWork;

		struct SPacketData
		{
			netid rcvNetId;	// 패킷을 받은 서버의 netid
			CPacket packet;
			SPacketData() {}
			SPacketData(netid netId, const CPacket &p) : rcvNetId(netId), packet(p) {}
		};

	protected:
		typedef std::list<SPacketData> PacketQueue;
		typedef PacketQueue::iterator PacketItor;

		PacketQueue			m_Packets;				// 네트워크로부터 받은 패킷을 저장한다.
		CRITICAL_SECTION	m_CriticalSection;

	protected:
		void PushPacket(const SPacketData &data)
		{
			EnterSync();
			m_Packets.push_back(data);
			LeaveSync();
		}

		bool PopPacket(OUT SPacketData &data)
		{
			EnterSync();
			if (m_Packets.empty())
			{
				LeaveSync();
				return false;
			}
			data = m_Packets.front();
			m_Packets.pop_front();
			LeaveSync();
			return true;
		}

		void Clear()
		{
			DeleteCriticalSection(&m_CriticalSection);
		}

		void EnterSync()
		{
			EnterCriticalSection(&m_CriticalSection);
		}

		void LeaveSync()
		{
			LeaveCriticalSection(&m_CriticalSection);
		}
	};
}
