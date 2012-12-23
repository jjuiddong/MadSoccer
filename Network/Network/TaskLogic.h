//------------------------------------------------------------------------
// Name:    TaskLogic.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// 네트워크로부터 받은 패킷을 파싱해서 Listener에 등록된 객체에게 메세지를
// 보낸다.
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
