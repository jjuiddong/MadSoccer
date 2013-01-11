//------------------------------------------------------------------------
// Name:    NetController.h
// Author:  jjuiddong
// Date:    12/24/2012
// 
// ��Ʈ��ũ�� �ʿ��� ������� ����,Ŭ���̾�Ʈ ��ü�� �����Ѵ�.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class CNetController : public common::CSingleton<CNetController>
	{
	public:
		CNetController();
		virtual ~CNetController();
		friend class CTaskAccept;

	protected:
		typedef std::map<SOCKET, ServerPtr> ServerMap;
		typedef std::map<SOCKET, ClientPtr> ClientMap;
		typedef std::map<int,IProtocolDispatcher*> DispatcherMap;
		typedef ServerMap::iterator ServerItor;
		typedef ClientMap::iterator ClientItor;
		typedef DispatcherMap::iterator DispatcherItor;
		typedef std::list<common::CThread*> ThreadList;
		typedef ThreadList::iterator ThreadItor;

		ServerMap			m_Servers;
		ClientMap			m_Clients;
		DispatcherMap		m_Dipatchers;
		common::CThread		m_AcceptThread;
		ThreadList			m_WorkThreads;
		ThreadList			m_LogicThreads;
		CRITICAL_SECTION	m_CriticalSection;

	public:
		bool		Init(int logicThreadCount);
		void		Proc();
		void		Clear();

		// server
		bool		StartServer(int port, ServerPtr pSvr);
		bool		StopServer(CServer *pSvr);
		ServerPtr	GetServer(SOCKET sock);

		// client
		bool		StartClient(const std::string &ip, int port, ClientPtr pClt);
		bool		StopClient(CClient *pClt);
		ClientPtr	GetClient(SOCKET sock);

		// protocol
		void		AddDispatcher(IProtocolDispatcher *pDispatcher);
		IProtocolDispatcher* GetDispatcher(int protocolID);

		// debug
		std::string ToString();

	protected:
		void		MakeServersFDSET( fd_set *pfdset);
		void		EnterSync();
		void		LeaveSync();

	};
}
