//------------------------------------------------------------------------
// Name:    Server.h
// Author:  jjuiddong
// Date:    2012-11-27
// 
// CClient�� ������ Server�� ǥ���Ѵ�.
//------------------------------------------------------------------------
#pragma once


namespace network
{
	class CServer
	{
	public:
		CServer();
		virtual ~CServer();

	protected:
		int					m_Id;					// ���� ����ID
		SOCKET				m_ListenSocket;
		int					m_ServerPort;
		bool				m_IsServerOn;			// ������ ���������� ������ �Ǿ��ٸ� true
		CRITICAL_SECTION	m_CriticalSection;

		SocketList			m_ClientSockets;
		PacketList			m_Packets;

		ListenerList		m_Listners;
		IPacketDispatcher	*m_pDispatcher;

		common::CThread		m_AcceptThread;
		common::CThread		m_RecvThread;
		common::CThread		m_WorkThread;

	public:
		bool				Start(int port);
		bool				Stop();

		int					GetId() const { return m_Id; }
		SOCKET				GetListenSocket() { return m_ListenSocket; }
		bool				IsServerOn() { return m_IsServerOn; }
		const SocketList&	GetClientSockets() { return m_ClientSockets; }
		bool				IsExist(SOCKET socket);
		const PacketList&	GetPackets() { return m_Packets; }
		void				MakeFDSET( fd_set *pfdset);
		void				EnterSync();
		void				LeaveSync();

		bool				AddClient(SOCKET sock);
		bool				RemoveClient(SOCKET sock);
		SockItor			RemoveClientInLoop(SOCKET sock);
		bool				PushPacket(const CPacket &packet);
		bool				Send(SOCKET sock, const CPacket &packet);
		bool				SendAll(const CPacket &packet);
		void				ClearPackets();
		void				Clear();

		bool				AddListener(IPacketListener *listener);
		bool				RemoveListener(IPacketListener *listener);
		const ListenerList&	GetListeners() const { return m_Listners; }

	public:
		virtual void		ProcessPacket( const CPacket &rcvPacket );

	protected:
		// Overriding
		virtual void		OnListen();
		virtual void		OnClientJoin(SOCKET sock) {}
		virtual void		OnClientLeave(SOCKET sock) {}

	};

};
