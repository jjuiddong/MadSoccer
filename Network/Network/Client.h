//------------------------------------------------------------------------
// Name:    Client.h
// Author:  jjuiddong
// Date:    2012-11-27
//
// CServer�� �����ϴ� Client�� ǥ���Ѵ�.
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
		int			m_Id;				// Ŭ���̾�Ʈ ���̵�
		std::string	m_ServerIP;
		int			m_ServerPort;
		SOCKET		m_Socket;			// Socket
		bool		m_IsConnect;

	public:
		bool		Start(std::string ip, int port);
		bool		Stop();
		void		Clear();
		bool		Proc();

		int			GetId()	const { return m_Id; }
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

};
