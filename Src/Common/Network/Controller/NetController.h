/**
Name:   NetController.h
Author:  jjuiddong
Date:    12/24/2012


��Ʈ��ũ�� �ʿ��� ������� ����,Ŭ���̾�Ʈ ��ü�� �����Ѵ�.
Dipatcher �� �����Ѵ�.

*/
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
		friend class CTaskLogic;
		friend class CTaskAccept;
		friend class CTaskWorkClient;
		friend class CTaskWorkServer;

		//typedef std::map<SOCKET, ServerBasicPtr> ServerSockets;
		//typedef std::map<SOCKET, ClientBasicPtr> ClientSockets;
		typedef std::map<int,IProtocolDispatcher*> DispatcherMap;

		//typedef ServerSockets::iterator ServerItor;
		//typedef ClientSockets::iterator ClientItor;
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
		//ServerBasicPtr	GetServerFromSocket(SOCKET sock);

		// Client
		bool		StartClient(const std::string &ip, int port, ClientBasicPtr pClt);
		bool		StopClient(ClientBasicPtr pClt);
		bool		RemoveClient(ClientBasicPtr pClt);
		ClientBasicPtr	GetClient(netid netId);
		//ClientBasicPtr	GetClientFromSocket(SOCKET sock);

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
		void		MakeServersFDSET( SFd_Set *pfdset);
		void		MakeCoreClientsFDSET( PROCESS_TYPE procType, SFd_Set *pfdset);
		void		DisconnectServer(ServerBasicPtr pSvr);
		void		DisconnectClient(ClientBasicPtr pClt);
		void		DisconnectCoreClient(CoreClientPtr pClt);
		void		MainLoop();
		void		RemoveProcess();

	private:
		Servers							m_Servers;
		//ServerSockets				m_ServerSockets;

		Clients							m_Clients;
		//ClientSockets				m_ClientSockets;

		CoreClients					m_CoreClients;

		DispatcherMap				m_Dispatchers;
		common::CThread		m_AcceptThread;

		/// CServer�� SERVICE_THREAD Ÿ���϶� �߰��ȴ�.
		/// Thread �ν��Ͻ��� m_WorkThreads �� ����ȴ�.
		ThreadPtr						m_pSeperateServerWorkThread;

		/// CCoreClient�� SERVICE_THREAD Ÿ���϶� �߰��ȴ�.
		/// Thread �ν��Ͻ��� m_WorkThreads �� ����ȴ�.
		ThreadPtr						m_pSeperateClientWorkThread;	

		ThreadList					m_WorkThreads;
		ThreadList					m_LogicThreads;
		common::CriticalSection  m_CS;
		int								m_UniqueValue;

	};


	inline int	CNetController::GetUniqueValue() const { return m_UniqueValue; }

}
