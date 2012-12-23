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
			if (!CLogicThreadAllocator::Get()->PopPacket(packetData))
				return RR_CONTINUE;

			CServer *pSvr = GetServer(packetData.serverId);
			if (!pSvr)
			{
				error::ErrorLog( common::format("%d �� �ش��ϴ� ������ �����ϴ�.", packetData.serverId) );
				return RR_CONTINUE;
			}

			IPacketDispatcher *pDispatcher = pSvr->GetDispatcher();
			if (!pDispatcher)
			{
				error::ErrorLog( "Dispatcher�� �������� �ʾҽ��ϴ�." );
				return RR_CONTINUE;
			}

			// ��Ŷ�� ��ġ�ϴ� �������̽��� ȣ���Ѵ�.
			pDispatcher->Dispatch(packetData.packet, pSvr->GetListeners() );

			return RR_CONTINUE;
		}
	};

}
