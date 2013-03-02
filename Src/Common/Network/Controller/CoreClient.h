//------------------------------------------------------------------------
// Name:    ClientCore.h
// Author:  jjuiddong
// Date:    2/28/2013
// 
// CServer 에 접속하는 클라이언트 클래스다. 
//------------------------------------------------------------------------
#pragma once

#include "NetConnector.h"

namespace network
{
	class CCoreClient : public CNetConnector
	{
	public:
		CCoreClient();
		virtual ~CCoreClient();
		friend class CNetLauncher;

	protected:
		std::string	m_ServerIP;
		int				m_ServerPort;
		bool				m_IsConnect;

	public:
		bool				Stop();
		bool				Proc();
		void				Clear();

		bool					IsConnect() const { return m_IsConnect; }
		virtual bool		Send(netid netId, const CPacket &packet) override;
		virtual bool		SendAll(const CPacket &packet) override;

	protected:
		void				SetConnect(bool isConnect) { m_IsConnect = isConnect; }
		void				SetServerIp(const std::string &ip) { m_ServerIP = ip; }
		void				SetServerPort(int port) { m_ServerPort = port; }
		void				Disconnect();

		// oeverriding
		virtual void		OnConnect() {}
		virtual void		OnDisconnect() {}
		virtual void		OnMemberJoin() {}
		virtual void		OnMemberLeave() {}

	};
}
