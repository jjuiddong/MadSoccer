//------------------------------------------------------------------------
// Name:    AcceptTask.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// 클라이언트를 받아서 소켓을 서버에게 넘겨준다.
//------------------------------------------------------------------------

#pragma once


namespace network
{
	class CAcceptTask : public common::CTask
	{
	public:
		CAcceptTask(CServer*psvr) : CTask(0), m_pServer(psvr) {}
		virtual ~CAcceptTask() {}

	protected:
		common::ReferencePtr<CServer>	m_pServer;

	public:
		void	SetServerPtr(CServer *psvr) { m_pServer = psvr; }

		virtual RUN_RESULT	Run() override
		{
			if (!m_pServer) return RR_CONTINUE;

			// 오는 요청을 기다립니다.
			SOCKET remoteSocket;

			// accept(요청을 받으 소켓, 선택 클라이언트 주소)
			remoteSocket = accept(m_pServer->GetListenSocket(), NULL, NULL);
			if(remoteSocket == INVALID_SOCKET)
			{
				//PRINTERROR("accept()");
				//closesocket(m_ListenSocket);
				return RR_END;
			}

 			m_pServer->EnterSync();
 			m_pServer->AddClient( remoteSocket );
 			m_pServer->LeaveSync();

			return RR_CONTINUE; 
		}
	};

}
