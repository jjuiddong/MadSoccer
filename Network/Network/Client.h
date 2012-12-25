//------------------------------------------------------------------------
// Name:    Client.h
// Author:  jjuiddong
// Date:    2012-11-27
//
// CServer에 접속하는 Client를 표현한다.
//------------------------------------------------------------------------

#pragma once

namespace network
{
	class CClient
	{
	public:
		CClient();
		virtual ~CClient();
		friend class CNetLauncher;

	protected:
		int			m_Id;				// 클라이언트 아이디 (자동생성)
		std::string	m_ServerIP;
		int			m_ServerPort;
		SOCKET		m_Socket;			// Socket
		bool		m_IsConnect;

	public:
		bool		Stop();
		void		Clear();
		bool		Proc();

		int			GetId()	const { return m_Id; }
		SOCKET		GetSocket() const { return m_Socket; }
		bool		IsConnect() const { return m_IsConnect; }
		bool		Send(const CPacket &packet);

	protected:
		void		SetConnect(bool isConnect) { m_IsConnect = isConnect; }
		void		SetSocket(SOCKET sock) { m_Socket = sock; }
		void		SetServerIp(const std::string &ip) { m_ServerIP = ip; }
		void		SetServerPort(int port) { m_ServerPort = port; }

		// oeverriding
		virtual void ProcessPacket( const CPacket &rcvPacket ) {}
		virtual void OnConnect() {}
		virtual void OnDisconnect();
		virtual void OnMemberJoin() {}
		virtual void OnMemberLeave() {}

	};

};
