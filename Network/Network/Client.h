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

	public:
		virtual bool Start();
		virtual bool Stop();
		virtual void Connect();
		virtual void OnMemberJoin();
		virtual void OnMemberLeave();
		virtual bool Proc();

	};

};
