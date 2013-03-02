//------------------------------------------------------------------------
// Name:    NetController.h
// Author:  jjuiddong
// Date:    12/24/2012
// 
// 네트워크에 필요한 쓰레드와 서버,클라이언트 객체를 관리한다.
// Dipatcher 를 관리한다.
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
		enum {
			VECTOR_RESERVED_SIZE = 128,
		};

		typedef std::map<SOCKET, ServerPtr> ServerSockets;
		typedef common::VectorMap<netid, ServerPtr> Servers;

		typedef std::map<SOCKET, ClientPtr> ClientSockets;
		typedef common::VectorMap<netid, ClientPtr> Clients;
		typedef common::VectorMap<netid, CoreClientPtr> CoreClients;

		typedef std::map<int,IProtocolDispatcher*> DispatcherMap;
		typedef ServerSockets::iterator ServerItor;
		typedef ClientSockets::iterator ClientItor;
		typedef DispatcherMap::iterator DispatcherItor;
		typedef std::list<common::CThread*> ThreadList;
		typedef ThreadList::iterator ThreadItor;

		Servers							m_Servers;
		ServerSockets				m_ServerSockets;

		Clients							m_Clients;
		ClientSockets				m_ClientSockets;

		CoreClients					m_CoreClients;

		DispatcherMap				m_Dipatchers;
		common::CThread		m_AcceptThread;
		ThreadList					m_WorkThreads;
		ThreadList					m_LogicThreads;
		CRITICAL_SECTION		m_CriticalSection;

	public:
		bool		Init(int logicThreadCount);
		void		Proc();
		void		Clear();

		// Server
		bool		StartServer(int port, ServerPtr pSvr);
		bool		StopServer(CServer *pSvr);
		ServerPtr	GetServer(netid netId);
		ServerPtr	GetServerFromSocket(SOCKET sock);

		// Client
		bool		StartClient(const std::string &ip, int port, ClientPtr pClt);
		bool		StopClient(CClient *pClt);
		ClientPtr	GetClient(netid netId);
		ClientPtr	GetClientFromSocket(SOCKET sock);

		// CoreClient
		bool		StartCoreClient(const std::string &ip, int port, CoreClientPtr pClt);
		bool		StopCoreClient(CCoreClient *pClt);
		CoreClientPtr	GetCoreClient(netid netId);

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
