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
	DECLARE_TYPE_NAME_SCOPE(network, CNetController)
	class CNetController : public common::CSingleton<CNetController>
										, public memmonitor::Monitor<CNetController, TYPE_NAME(network::CNetController)>
	{
	public:
		CNetController();
		virtual ~CNetController();
		friend class CTaskAccept;
		friend class CTaskWorkClient;
		friend class CTaskWorkServer;

		typedef std::map<SOCKET, ServerBasicPtr> ServerSockets;
		typedef common::VectorMap<netid, ServerBasicPtr> Servers;

		typedef std::map<SOCKET, ClientBasicPtr> ClientSockets;
		typedef common::VectorMap<netid, ClientBasicPtr> Clients;
		typedef common::VectorMap<netid, CoreClientPtr> CoreClients;

		typedef std::map<int,IProtocolDispatcher*> DispatcherMap;
		typedef ServerSockets::iterator ServerItor;
		typedef ClientSockets::iterator ClientItor;
		typedef DispatcherMap::iterator DispatcherItor;
		typedef std::list<common::CThread*> ThreadList;
		typedef ThreadList::iterator ThreadItor;

		enum { VECTOR_RESERVED_SIZE = 128, };

		bool		Init(int logicThreadCount);
		void		Proc();
		void		Clear();

		// Server
		bool		StartServer(int port, ServerBasicPtr pSvr);
		bool		StopServer(ServerBasicPtr pSvr);
		bool		RemoveServer(ServerBasicPtr pSvr);
		ServerBasicPtr	GetServer(netid netId);
		ServerBasicPtr	GetServerFromSocket(SOCKET sock);

		// Client
		bool		StartClient(const std::string &ip, int port, ClientBasicPtr pClt);
		bool		StopClient(ClientBasicPtr pClt);
		bool		RemoveClient(ClientBasicPtr pClt);
		ClientBasicPtr	GetClient(netid netId);
		ClientBasicPtr	GetClientFromSocket(SOCKET sock);

		// CoreClient
		bool		StartCoreClient(const std::string &ip, int port, CoreClientPtr pClt);
		bool		StopCoreClient(CoreClientPtr  pClt);
		bool		RemoveCoreClient(CoreClientPtr  pClt);
		CoreClientPtr	GetCoreClient(netid netId);

		// protocol
		void		AddDispatcher(IProtocolDispatcher *pDispatcher);
		IProtocolDispatcher* GetDispatcher(int protocolID);

		int		GetUniqueValue() const;

		// debug
		std::string ToString();

	protected:
		ThreadPtr AllocWorkThread(SERVICE_TYPE serviceType, NetConnectorPtr pConnector);
		ThreadPtr GetThread( const ThreadList &threads, HANDLE hThreadHandle );
		void		MakeServersFDSET( fd_set *pfdset);
		void		MakeCoreClientsFDSET( PROCESS_TYPE procType, SFd_Set *pfdset);
		void		DisconnectServer(ServerBasicPtr pSvr);
		void		DisconnectClient(ClientBasicPtr pClt);
		void		DisconnectCoreClient(CoreClientPtr pClt);

	protected:
		Servers							m_Servers;
		ServerSockets				m_ServerSockets;

		Clients							m_Clients;
		ClientSockets				m_ClientSockets;

		CoreClients					m_CoreClients;

		DispatcherMap				m_Dispatchers;
		common::CThread		m_AcceptThread;

		/// CServer가 SERVICE_THREAD 타입일때 추가된다.
		/// Thread 인스턴스는 m_WorkThreads 에 저장된다.
		ThreadPtr						m_pSeperateServerWorkThread;

		/// CCoreClient가 SERVICE_THREAD 타입일때 추가된다.
		/// Thread 인스턴스는 m_WorkThreads 에 저장된다.
		ThreadPtr						m_pSeperateClientWorkThread;	

		ThreadList					m_WorkThreads;
		ThreadList					m_LogicThreads;
		common::CriticalSection  m_CS;
		int								m_UniqueValue;

	};


	inline int	CNetController::GetUniqueValue() const { return m_UniqueValue; }

}
