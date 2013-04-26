/**
Name:   ServerBasic.h
Author:  jjuiddong
Date:    2012-11-27

클라이언트가 접속할 Server를 표현한다.
*/
#pragma once

namespace network
{

	class CServerBasic : public CNetConnector
	{
		friend class CNetLauncher;
		friend class CNetController;
		friend class CTaskLogic;
		friend class CP2PClient;

	public:
		CServerBasic(PROCESS_TYPE procType);
		virtual ~CServerBasic();

		bool				AddSession(SOCKET sock, const std::string &ip);
		CSession*	GetSession(netid netId);
		CSession*	GetSession(const std::string &clientId);
		bool				RemoveSession(netid netId);
		bool				RemoveSessionSocket(netid netId);

		CGroup&		GetRootGroup();
		Sessions_V& GetSessions();
		void				SetSessionFactory( ISessionFactory *ptr );
		void				SetGroupFactory( IGroupFactory *ptr );
		ISessionFactory* GetSessionFactory() const;
		IGroupFactory* GetGroupFactory() const;

		common::CriticalSection& GetCS();
		netid			GetNetIdFromSocket(SOCKET sock);
		bool				IsExist(netid netId);
		void				Clear();

		void				Proc();
		bool				Stop();
		void				Disconnect();
		void				Close();
		bool				IsServerOn() const;
		void				MakeFDSET( SFd_Set *pfdset);
		void				SetPort(int port);
		void				AddTimer( int id, int intervalTime, bool isRepeat = true );
		void				KillTimer( int id );

		virtual bool	Send(netid netId, const SEND_FLAG flag, const CPacket &packet) override;
		virtual bool	SendAll(const CPacket &packet) override;

		// Event Handler
		void				OnListen();
		void				OnDisconnect();
		void				OnClientJoin(netid netId);
		void				OnClientLeave(netid netId);
		virtual void	OnTimer(int id);

	protected:
		void				MainLoop();
		void				InitRootGroup();
		bool				AcceptProcess();
		void				DispatchPacket();
		bool				SendGroup(GroupPtr pGroup, const CPacket &packet);
		bool				SendViewer(netid groupId, const SEND_FLAG flag, const CPacket &packet);
		bool				SendViewerRecursive(netid viewerId, const netid exceptGroupId, const CPacket &packet);

		bool				RemoveClientProcess(SessionItor it);
		SessionItor	FindSessionBySocket(SOCKET sock);

	private:
		int								m_ServerPort;
		bool								m_IsServerOn;			// 서버가 정상적으로 실행이 되었다면 true
		Sessions_						m_Sessions;					// 서버와 연결된 클라이언트 정보리스트
		ISessionFactory			*m_pSessionFactory;
		IGroupFactory			    *m_pGroupFactory;
		common::CriticalSection  m_CS;
		std::vector<STimer>	m_Timers;

		CGroup							m_RootGroup;
		netid							m_WaitGroupId;			// waiting place before join concrete group
	};


	inline bool CServerBasic::IsServerOn() const { return m_IsServerOn; }
	inline void	 CServerBasic::SetPort(int port) { m_ServerPort = port; }
	inline CGroup&	CServerBasic::GetRootGroup() { return m_RootGroup; }
	inline common::CriticalSection& CServerBasic::GetCS() { return m_CS; }
	inline Sessions_V& CServerBasic::GetSessions() { return m_Sessions.m_Seq; }
	inline ISessionFactory* CServerBasic::GetSessionFactory() const { return m_pSessionFactory; }
	inline IGroupFactory* CServerBasic::GetGroupFactory() const { return m_pGroupFactory; }

};
