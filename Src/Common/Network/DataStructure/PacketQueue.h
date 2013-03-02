//------------------------------------------------------------------------
// Name:    PacketQueue.h
// Author:  jjuiddong
// Date:    12/24/2012
// 
// ��Ʈ��ũ�� ���� ���� ��Ŷ�� �����Ѵ�. ��� ������ Ŭ���̾�Ʈ�� �ϳ��� 
// ��Ŷť�� ����ȴ�.
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
			netid rcvNetId;	// ��Ŷ�� ���� ������ netid
			CPacket packet;
			SPacketData() {}
			SPacketData(netid netId, const CPacket &p) : rcvNetId(netId), packet(p) {}
		};

	protected:
		typedef std::list<SPacketData> PacketQueue;
		typedef PacketQueue::iterator PacketItor;

		PacketQueue			m_Packets;				// ��Ʈ��ũ�κ��� ���� ��Ŷ�� �����Ѵ�.
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
