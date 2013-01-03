//------------------------------------------------------------------------
// Name:    NetController.h
// Author:  jjuiddong
// Date:    12/24/2012
// 
// 네트워크에 필요한 쓰레드와 서버,클라이언트 객체를 관리한다.
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
		typedef std::map<SOCKET, CServer*> ServerMap;
		typedef std::map<SOCKET, CClient*> ClientMap;
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
		bool		StartServer(int port, CServer *pSvr);
		bool		StopServer(CServer *pSvr);
		CServer*	GetServer(SOCKET sock);

		// client
		bool		StartClient(const std::string &ip, int port, CClient *pClt);
		bool		StopClient(CClient *pClt);
		CClient*	GetClient(SOCKET sock);

		// protocol
		void		AddDispatcher(IProtocolDispatcher *pDispatcher);
		IProtocolDispatcher* GetDispatcher(int protocolID);

	protected:
		void		MakeServersFDSET( fd_set *pfdset);
		void		EnterSync();
		void		LeaveSync();

	};
}
