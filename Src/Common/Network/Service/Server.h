//------------------------------------------------------------------------
// Name:    Server.h
// Author:  jjuiddong
// Date:    2012-11-27
// 
// 클라이언트가 접속할 Server를 표현한다.
//------------------------------------------------------------------------
#pragma once

#include "Controller/NetConnector.h"
#include "../interface/ServerEventListener.h"
#include "NetCommon/Src/basic_ProtocolListener.h"
#include "NetCommon/Src/basic_Protocol.h"

namespace network
{
	class IServerEventListener;
	class CServer : public CNetConnector
							, public basic::c2s_ProtocolListener
	{
		friend class CNetLauncher;
		friend class CNetController;
		typedef common::ReferencePtr<IServerEventListener> ServerEventListenerPtr;

	public:
		CServer(PROCESS_TYPE procType);
		virtual ~CServer();

		bool				AddClient(SOCKET sock);
		CRemoteClient* GetRemoteClient(netid netId);
		bool				RemoveClient(netid netId);
		RemoteClientItor	RemoveClientInLoop(netid netId);
		netid			GetNetIdFromSocket(SOCKET sock);
		void				Clear();

		bool				Stop();
		void				Disconnect();

		virtual bool	Send(netid netId, const CPacket &packet) override;
		virtual bool	SendAll(const CPacket &packet) override;

		bool				IsServerOn() const;
		PROCESS_TYPE	GetProcessType() const;
		void					SetThreadHandle(HANDLE handle);
		HANDLE			GetThreadHandle() const;

		bool				IsExist(netid netId);
		void				SetEventListener(ServerEventListenerPtr ptr) { m_pEventListener = ptr; }
		void				MakeFDSET( SFd_Set *pfdset);
		void				EnterSync();
		void				LeaveSync();

	protected:
		void				SetPort(int port) { m_ServerPort = port; }
		RemoteClientItor	RemoveClientProcess(RemoteClientItor it);
		RemoteClientItor	FindRemoteClientBySocket(SOCKET sock);

		// Event Handler
		void				OnListen();
		void				OnClientJoin(netid netId);
		void				OnClientLeave(netid netId);

		// Network Handler
		virtual void ReqGroupList(netid senderId, const netid &groupid) override;
		virtual void ReqGroupJoin(netid senderId, const netid &groupid) override;
		virtual void ReqP2PConnect(netid senderId) override;

	protected:
		PROCESS_TYPE			m_ProcessType;
		int								m_ServerPort;
		bool								m_IsServerOn;			// 서버가 정상적으로 실행이 되었다면 true
		RemoteClientMap		m_RemoteClients;		// 서버와 연결된 클라이언트 정보리스트
		CRITICAL_SECTION		m_CriticalSection;
		ServerEventListenerPtr m_pEventListener;
		HANDLE						m_hThread;					// 소속된 스레드 핸들, 없다면 NULL

		CGroup							m_RootGroup;
		netid							m_WaitGroupId;			// waiting place before join concrete group

		// protocols
		basic::s2c_Protocol		m_BasicProtocol;

	};

	inline bool CServer::IsServerOn() const { return m_IsServerOn; }
	inline PROCESS_TYPE CServer::GetProcessType() const { return m_ProcessType; }
	inline void	 CServer::SetThreadHandle(HANDLE handle) { m_hThread = handle; }
	inline HANDLE CServer::GetThreadHandle() const { return m_hThread; }

};
