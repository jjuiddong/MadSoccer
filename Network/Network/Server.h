//------------------------------------------------------------------------
// Name:    Server.h
// Author:  jjuiddong
// Date:    2012-11-27
// 
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
		typedef std::list<SOCKET> SockList;
		typedef SockList::iterator SockItor;

		SOCKET		m_ListenSocket;
		int			m_ServerPort;
		SockList	m_SockList;

	protected:
		bool AddClient(SOCKET sock);
		bool RemoveClient(SOCKET sock);
		void Clear();

	public:
		virtual bool Start();
		virtual bool Stop();
		virtual void OnListen();
		virtual void OnClientJoin();
		virtual void OnClientLeave();
		virtual void Recv();
		virtual bool Proc();

	};

};
