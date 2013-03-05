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
		friend class CNetLauncher;
		friend class CNetController;
		friend class CClient;

	protected:
		PROCESS_TYPE m_ServiceType;
		std::string	m_ServerIP;
		int				m_ServerPort;
		bool				m_IsConnect;

	public:
		CCoreClient(PROCESS_TYPE serviceType);
		virtual ~CCoreClient();

		PROCESS_TYPE GetProcessType() const { return m_ServiceType; }
		bool					IsConnect() const { return m_IsConnect; }

		bool					Stop();
		void					Disconnect();

		virtual bool		Send(netid netId, const CPacket &packet) override;
		virtual bool		SendAll(const CPacket &packet) override;

	protected:
		bool				Proc();
		void				Clear();
		void				SetConnect(bool isConnect) { m_IsConnect = isConnect; }
		void				SetServerIp(const std::string &ip) { m_ServerIP = ip; }
		void				SetServerPort(int port) { m_ServerPort = port; }

		// oeverriding
		virtual void		OnConnect() {}
		virtual void		OnDisconnect() {}
		virtual void		OnMemberJoin() {}
		virtual void		OnMemberLeave() {}

	};
}
