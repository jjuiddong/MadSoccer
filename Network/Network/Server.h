//------------------------------------------------------------------------
// Name:    Server.h
// Author:  jjuiddong
// Date:    2012-11-27
// 
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

	protected:
		SOCKET		m_ListenSocket;
		int			m_ServerPort;
		bool		m_IsServerOn;

		SocketList	m_ClientSockets;
		PacketList	m_Packets;

	protected:

	public:
		bool				Start(int port);
		bool				Stop();

		SOCKET				GetListenSocket() { return m_ListenSocket; }
		bool				IsServerOn() { return m_IsServerOn; }
		const SocketList&		GetClientSockets() { return m_ClientSockets; }
		bool				IsExist(SOCKET socket);
		const PacketList&	GetPackets() { return m_Packets; }
		void				MakeFDSET( fd_set *pfdset);

		bool				AddClient(SOCKET sock);
		bool				RemoveClient(SOCKET sock);
		SockItor			RemoveClientInLoop(SOCKET sock);
		bool				PushPacket(const CPacket &packet);
		bool				Send(SOCKET sock, const CPacket &packet);
		bool				SendAll(const CPacket &packet);
		void				ClearPackets();
		void				Clear();

	public:
		virtual void		ProcessPacket( const CPacket &rcvPacket );

	protected:
		// Overriding
		virtual void		OnListen();
		virtual void		OnClientJoin(SOCKET sock) {}
		virtual void		OnClientLeave(SOCKET sock) {}

	};

};
