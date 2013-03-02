//------------------------------------------------------------------------
// Name:    Client.h
// Author:  jjuiddong
// Date:    2012-11-27
//
// CServer에 접속하는 Client를 표현한다.
// P2P 통신 기능
//------------------------------------------------------------------------
#pragma once

#include "Controller/NetConnector.h"

namespace network
{
	class CClient : public CNetConnector
	{
	public:
		CClient();
		virtual ~CClient();
		friend class CNetLauncher;

	protected:
		std::string	m_ServerIP;
		int				m_ServerPort;
		bool				m_IsConnect;
// 		CClientCore	*m_pP2pOnly;
// 		CServer		*m_pP2p
//		CClientCore	*m_pSvrConnectOnly;

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
};
