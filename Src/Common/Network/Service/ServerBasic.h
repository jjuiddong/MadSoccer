//------------------------------------------------------------------------
// Name:    ServerBasic.h
// Author:  jjuiddong
// Date:    2012-11-27
// 
// Ŭ���̾�Ʈ�� ������ Server�� ǥ���Ѵ�.
//------------------------------------------------------------------------
#pragma once

#include "Controller/NetConnector.h"
#include "../interface/ServerEventListener.h"

namespace network
{
	class IServerEventListener;
	class CServerBasic : public CNetConnector
	{
		friend class CNetLauncher;
		friend class CNetController;
		typedef common::ReferencePtr<IServerEventListener> ServerEventListenerPtr;

	public:
		CServerBasic(PROCESS_TYPE procType);
		virtual ~CServerBasic();

		bool				AddRemoteClient(SOCKET sock, const std::string &ip);
		CRemoteClient* GetRemoteClient(netid netId);
		bool				RemoveRemoteClient(netid netId);
		RemoteClientItor	RemoveRemoteClientInLoop(netid netId);
		netid			GetNetIdFromSocket(SOCKET sock);
		bool				IsExist(netid netId);
		void				Clear();

		void				Proc();
		bool				Stop();
		void				Disconnect();

		virtual bool	Send(netid netId, const SEND_FLAG flag, const CPacket &packet);
		virtual bool	SendAll(const CPacket &packet) override;

		void				SetEventListener(ServerEventListenerPtr ptr) { m_pEventListener = ptr; }
		void				MakeFDSET( SFd_Set *pfdset);
		void				EnterSync();
		void				LeaveSync();

		bool				IsServerOn() const;
		PROCESS_TYPE	GetProcessType() const;
		void					SetThreadHandle(HANDLE handle);
		HANDLE			GetThreadHandle() const;

	protected:
		bool				SendGroup(GroupPtr pGroup, const CPacket &packet);
		bool				SendViewer(netid groupId, const SEND_FLAG flag, const CPacket &packet);
		bool				SendViewerRecursive(netid viewerId, const netid exceptGroupId, const CPacket &packet);

		void				SetPort(int port);
		RemoteClientItor	RemoveClientProcess(RemoteClientItor it);
		RemoteClientItor	FindRemoteClientBySocket(SOCKET sock);

		// Event Handler
		void				OnListen();
		void				OnClientJoin(netid netId);
		void				OnClientLeave(netid netId);

	protected:
		PROCESS_TYPE			m_ProcessType;
		int								m_ServerPort;
		bool								m_IsServerOn;			// ������ ���������� ������ �Ǿ��ٸ� true
		RemoteClientMap		m_RemoteClients;		// ������ ����� Ŭ���̾�Ʈ ��������Ʈ
		CRITICAL_SECTION		m_CriticalSection;
		ServerEventListenerPtr m_pEventListener;
		HANDLE						m_hThread;					// �Ҽӵ� ������ �ڵ�, ���ٸ� NULL

		CGroup							m_RootGroup;
		netid							m_WaitGroupId;			// waiting place before join concrete group
	};

	inline bool CServerBasic::IsServerOn() const { return m_IsServerOn; }
	inline PROCESS_TYPE CServerBasic::GetProcessType() const { return m_ProcessType; }
	inline void	 CServerBasic::SetThreadHandle(HANDLE handle) { m_hThread = handle; }
	inline HANDLE CServerBasic::GetThreadHandle() const { return m_hThread; }
	inline void	 CServerBasic::SetPort(int port) { m_ServerPort = port; }

};
