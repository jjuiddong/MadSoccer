//------------------------------------------------------------------------
// Name:    TaskLogic.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// ��Ʈ��ũ�κ��� ���� ��Ŷ�� �Ľ��ؼ� Listener�� ��ϵ� ��ü���� �޼�����
// ������.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class CTaskLogic : public common::CTask
	{
	public:
		CTaskLogic() : CTask(0) {}

	public:
		virtual RUN_RESULT	Run() override
		{
			CLogicThreadAllocator::SPacketData packetData;
			if (CLogicThreadAllocator::Get()->PopPacket(packetData))
			{
				CServer *pSvr = GetServer(packetData.serverId);
				if (!pSvr) return RR_CONTINUE;

				


			}
			return RR_CONTINUE;
		}
	};

}
