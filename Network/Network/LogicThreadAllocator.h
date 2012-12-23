//------------------------------------------------------------------------
// Name:    LogicThreadAllocator.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// ���������带 �����ؼ�, ���� ��Ŷ�� ó���Ѵ�.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class CLogicThreadAllocator : public common::CSingleton<CLogicThreadAllocator>
	{
	public:
		CLogicThreadAllocator();
		virtual ~CLogicThreadAllocator();

		typedef struct _SPacketData
		{
			int serverId;
			CPacket packet;
			_SPacketData() {}
			_SPacketData(int svrid, const CPacket &p) : serverId(svrid), packet(p) {}
		} SPacketData;

	protected:
		typedef std::list<SPacketData> PacketQueue;
		typedef PacketQueue::iterator PacketItor;
		typedef std::list<common::CThread*> ThreadList;
		typedef ThreadList::iterator ThreadItor;

		ThreadList			m_Threads;
		PacketQueue			m_Packets;				// ��Ʈ��ũ�κ��� ���� ��Ŷ�� �����Ѵ�.
		CRITICAL_SECTION	m_CriticalSection;
		int					m_LogicThreadCount;

	public:
		void				Init( int logicThreadCount );
		void				PushPacket(const SPacketData &data);
		bool				PopPacket(OUT SPacketData &data);
		void				Clear();

	protected:
		void				EnterSync();
		void				LeaveSync();

	};
}
