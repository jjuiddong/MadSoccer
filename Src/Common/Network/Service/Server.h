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

namespace network
{
	class IServerEventListener;
	class CServer : public CNetConnector
	{
		friend class CNetLauncher;
		friend class CNetController;

		typedef common::ReferencePtr<IServerEventListener> ServerEventListenerPtr;

	public:
		CServer(PROCESS_TYPE procType);
		virtual ~CServer();

		bool				IsServerOn() const { return m_IsServerOn; }
		PROCESS_TYPE	GetProcessType() const { return m_ProcessType; }
		bool				IsExist(netid netId);
		void				SetEventListener(ServerEventListenerPtr ptr) { m_pEventListener = ptr; }
		void				MakeFDSET( SFd_Set *pfdset);
		void				EnterSync();
		void				LeaveSync();

		bool				AddClient(SOCKET sock);
		CRemoteClient* GetRemoteClient(netid netId);
		CRemoteClient* GetRemoteClientFromSocket(SOCKET sock);
		bool				RemoveClient(netid netId);
		bool				RemoveClientBySocket(SOCKET sock);
		RemoteClientItor	RemoveClientInLoop(netid netId);
		netid			GetNetIdFromSocket(SOCKET sock);
		void				Clear();

		bool				Stop();
		void				Disconnect();

		virtual bool	Send(netid netId, const CPacket &packet) override;
		virtual bool	SendAll(const CPacket &packet) override;

	protected:
		void				SetPort(int port) { m_ServerPort = port; }
		RemoteClientItor	RemoveClientProcess(RemoteClientItor it);
		RemoteClientItor	FindRemoteClientBySocket(SOCKET sock);

		// event
		void				OnListen();
		void				OnClientJoin(netid netId);
		void				OnClientLeave(netid netId);

	protected:
		PROCESS_TYPE			m_ProcessType;
		int								m_ServerPort;
		bool								m_IsServerOn;			// 서버가 정상적으로 실행이 되었다면 true
		RemoteClientMap		m_RemoteClients;		// 서버와 연결된 클라이언트 정보리스트
		CRITICAL_SECTION		m_CriticalSection;
		ServerEventListenerPtr m_pEventListener;

	};

};
