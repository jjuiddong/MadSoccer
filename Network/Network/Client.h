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

	protected:
		std::string	m_ServerIP;
		int			m_ServerPort;
		SOCKET		m_Socket;			// Socket
		bool		m_IsConnect;

	public:
		bool		Start(std::string ip, int port);
		bool		Stop();
		void		Clear();
		bool		Proc();

		bool		IsConnect() const { return m_IsConnect; }
		bool		Send(const CPacket &packet);

	protected:
		// oeverriding
		virtual void ProcessPacket( const CPacket &rcvPacket ) {}
		virtual void OnConnect() {}
		virtual void OnDisconnect();
		virtual void OnMemberJoin() {}
		virtual void OnMemberLeave() {}

	};


	// list<CClient*> 에서 CClient를 찾는 객체
	class IsClient : public std::unary_function<network::CClient*, bool>
	{
	public:
		IsClient(network::CClient *p):m_p(p) {  }
		network::CClient *m_p;
		bool operator ()(network::CClient *t) const
			{ return (t == m_p); }
	};
};
