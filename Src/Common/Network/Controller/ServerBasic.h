//------------------------------------------------------------------------
// Name:    ServerBasic.h
// Author:  jjuiddong
// Date:    2012-11-27
// 
// Ŭ���̾�Ʈ�� ������ Server�� ǥ���Ѵ�.
//------------------------------------------------------------------------
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

		bool				AddRemoteClient(SOCKET sock, const std::string &ip);
		CRemoteClient* GetRemoteClient(netid netId);
		bool				RemoveRemoteClient(netid netId);
		bool				RemoveRemoteClientSocket(netid netId);
		RemoteClientItor	RemoveRemoteClientInLoop(netid netId);
		CGroup&		GetRootGroup();
		RemoteClientMap& GetRemoteClients();
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

		virtual bool	Send(netid netId, const SEND_FLAG flag, const CPacket &packet) override;
		virtual bool	SendAll(const CPacket &packet) override;

		// Event Handler
		void				OnListen();
		void				OnClientJoin(netid netId);
		void				OnClientLeave(netid netId);

	protected:
		void				InitRootGroup();
		bool				AcceptProcess();
		void				DispatchPacket();
		bool				SendGroup(GroupPtr pGroup, const CPacket &packet);
		bool				SendViewer(netid groupId, const SEND_FLAG flag, const CPacket &packet);
		bool				SendViewerRecursive(netid viewerId, const netid exceptGroupId, const CPacket &packet);

		RemoteClientItor	RemoveClientProcess(RemoteClientItor it);
		RemoteClientItor	FindRemoteClientBySocket(SOCKET sock);

	private:
		int								m_ServerPort;
		bool								m_IsServerOn;			// ������ ���������� ������ �Ǿ��ٸ� true
		RemoteClientMap		m_RemoteClients;		// ������ ����� Ŭ���̾�Ʈ ��������Ʈ
		common::CriticalSection  m_CS;

		CGroup							m_RootGroup;
		netid							m_WaitGroupId;			// waiting place before join concrete group
	};


	inline bool CServerBasic::IsServerOn() const { return m_IsServerOn; }
	inline void	 CServerBasic::SetPort(int port) { m_ServerPort = port; }
	inline CGroup&	CServerBasic::GetRootGroup() { return m_RootGroup; }
	inline common::CriticalSection& CServerBasic::GetCS() { return m_CS; }
	inline RemoteClientMap& CServerBasic::GetRemoteClients() { return m_RemoteClients; }

};
