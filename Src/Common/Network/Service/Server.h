//------------------------------------------------------------------------
// Name:    Server.h
// Author:  jjuiddong
// Date:    2012-11-27
// 
// 클라이언트가 접속할 Server를 표현한다.
//------------------------------------------------------------------------
#pragma once

#include "Controller/NetConnector.h"

namespace network
{
	class CServer : public CNetConnector
	{
	public:
		CServer();
		virtual ~CServer();
		friend class CNetLauncher;

	protected:
		int								m_ServerPort;
		bool								m_IsServerOn;			// 서버가 정상적으로 실행이 되었다면 true
		RemoteClientMap		m_RemoteClients;		// 서버와 연결된 클라이언트 정보리스트
		CRITICAL_SECTION		m_CriticalSection;

	public:
		bool				Stop();

		bool				IsServerOn() const { return m_IsServerOn; }
		bool				IsExist(netid netId);
		void				MakeFDSET( fd_set *pfdset);
		void				EnterSync();
		void				LeaveSync();

		bool				AddClient(SOCKET sock);
		CRemoteClient*		GetRemoteClient(netid netId);
		CRemoteClient*		GetRemoteClientFromSocket(SOCKET sock);
		bool				RemoveClient(netid netId);
		bool				RemoveClientBySocket(SOCKET sock);
		RemoteClientItor	RemoveClientInLoop(netid netId);
		netid				GetNetIdFromSocket(SOCKET sock);
		void				Clear();

		virtual bool		Send(netid netId, const CPacket &packet) override;
		virtual bool		SendAll(const CPacket &packet) override;

	protected:
		void				SetPort(int port) { m_ServerPort = port; }
		RemoteClientItor	RemoveClientProcess(RemoteClientItor it);
		RemoteClientItor	FindRemoteClientBySocket(SOCKET sock);
		

		// Overriding
		virtual void		OnListen();
		virtual void		OnClientJoin(netid netId) {}
		virtual void		OnClientLeave(netid netId) {}

	};

};
