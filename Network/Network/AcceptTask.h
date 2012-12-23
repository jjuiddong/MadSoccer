//------------------------------------------------------------------------
// Name:    AcceptTask.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// Ŭ���̾�Ʈ�� �޾Ƽ� ������ �������� �Ѱ��ش�.
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

			// ���� ��û�� ��ٸ��ϴ�.
			SOCKET remoteSocket;

			// accept(��û�� ���� ����, ���� Ŭ���̾�Ʈ �ּ�)
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
