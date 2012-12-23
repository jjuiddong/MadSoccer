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
			if (!CLogicThreadAllocator::Get()->PopPacket(packetData))
				return RR_CONTINUE;

			CServer *pSvr = GetServer(packetData.serverId);
			if (!pSvr)
			{
				error::ErrorLog( common::format("%d 에 해당하는 서버가 없습니다.", packetData.serverId) );
				return RR_CONTINUE;
			}

			IPacketDispatcher *pDispatcher = pSvr->GetDispatcher();
			if (!pDispatcher)
			{
				error::ErrorLog( "Dispatcher가 설정되지 않았습니다." );
				return RR_CONTINUE;
			}

			// 패킷과 일치하는 인터페이스를 호출한다.
			pDispatcher->Dispatch(packetData.packet, pSvr->GetListeners() );

			return RR_CONTINUE;
		}
	};

}
