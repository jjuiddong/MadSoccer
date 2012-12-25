//------------------------------------------------------------------------
// Name:    Server.h
// Author:  jjuiddong
// Date:    2012-11-27
// 
// CClient가 접속할 Server를 표현한다.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class CServer
	{
	public:
		CServer();
		virtual ~CServer();
		friend class CNetLauncher;

	protected:
		int					m_Id;					// 서버 고유ID (자동생성)
		SOCKET				m_Socket;				// ListenSocket
		int					m_ServerPort;
		bool				m_IsServerOn;			// 서버가 정상적으로 실행이 되었다면 true
		SocketList			m_ClientSockets;

		ListenerList		m_Listners;
		ProtocolPtr			m_pProtocol;
		CRITICAL_SECTION	m_CriticalSection;

	public:
		bool				Stop();

		int					GetId() const { return m_Id; }
		SOCKET				GetSocket() { return m_Socket; }
		bool				IsServerOn() { return m_IsServerOn; }
		const SocketList&	GetClientSockets() { return m_ClientSockets; }
		bool				IsExist(SOCKET socket);
		void				MakeFDSET( fd_set *pfdset);
		void				EnterSync();
		void				LeaveSync();

		bool				AddClient(SOCKET sock);
		bool				RemoveClient(SOCKET sock);
		SockItor			RemoveClientInLoop(SOCKET sock);
		bool				Send(SOCKET sock, const CPacket &packet);
		bool				SendAll(const CPacket &packet);
		void				Clear();

		void				SetProtocol(ProtocolPtr protocol) { m_pProtocol = protocol; }
		ProtocolPtr			GetProtocol() const { return m_pProtocol; }
		bool				AddListener(IPacketListener *listener);
		bool				RemoveListener(IPacketListener *listener);
		const ListenerList&	GetListeners() const { return m_Listners; }

	protected:
		void				SetPort(int port) { m_ServerPort = port; }
		void				SetSocket(SOCKET sock) { m_Socket = sock; }

		// Overriding
		virtual void		OnListen();
		virtual void		OnClientJoin(SOCKET sock) {}
		virtual void		OnClientLeave(SOCKET sock) {}

	};

};
