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
	class CP2PClient;
	class CCoreClient;
	class CClient : public CNetConnector
	{
		friend class CNetLauncher;
		friend class CNetController;

	protected:
		PROCESS_TYPE m_ProcessType;		// m_pConnectSvr 와 값이 같다.

		CCoreClient	*m_pConnectSvr;			// p2p Server와 연결되는 CoreClient
		CP2PClient	*m_pP2p;

	public:
		CClient(PROCESS_TYPE procType);
		virtual ~CClient();

		PROCESS_TYPE	GetProcessType() const { return m_ProcessType; }
		CoreClientPtr	GetConnectSvrClient() const { return CoreClientPtr(m_pConnectSvr); }
		bool					IsConnect() const;

		bool					Stop();
		void					Disconnect();

		virtual bool		Send(netid netId, const CPacket &packet) override;
		virtual bool		SendAll(const CPacket &packet) override;

	protected:
		bool				Proc();
		void				Clear();

		// oeverriding
		virtual void		OnConnect() {}
		virtual void		OnDisconnect() {}
		virtual void		OnMemberJoin() {}
		virtual void		OnMemberLeave() {}

	};
};
