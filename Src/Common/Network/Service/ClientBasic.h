/**
Name:   ClientBasic.h
Author:  jjuiddong
Date:    3/17/2013

	CServer에 접속하는 Client를 표현한다.
	P2P 통신 기능
*/
#pragma once

#include "../interface/CoreClientEventListener.h"
#include "../interface/P2PClientEventListener.h"
#include "../interface/ClientEventListener.h"

namespace network
{
	class CP2PClient;
	class CCoreClient;
	class CClientBasic : public CNetConnector
		,public ICoreClientEventListener
		,public IP2PClientEventListener
	{

		friend class CNetLauncher;
		friend class CNetController;
		friend class CBasicS2CProtocolHandler;
		typedef common::ReferencePtr<IClientEventListener> ClientEventListenerPtr;

	public:
		CClientBasic(PROCESS_TYPE procType);
		virtual ~CClientBasic();

		bool					Proc();
		bool					Stop();
		void					Disconnect();
		void					Close();

		bool					IsConnect() const;
		bool					IsP2PHostClient() const;
		void					SetEventListener(ClientEventListenerPtr ptr);
		CoreClientPtr	GetConnectSvrClient() const;

		// Overriding
		virtual bool		AddProtocolListener(ProtocolListenerPtr pListener) override;
		virtual bool		RemoveProtocolListener(ProtocolListenerPtr pListener) override;
		virtual bool		Send(netid netId, const SEND_FLAG flag, const CPacket &packet) override;
		virtual bool		SendP2P(const CPacket &packet);
		virtual bool		SendAll(const CPacket &packet) override;

	protected:
		void					Clear();

		// CoreClient Event Handler
		virtual void		OnCoreClientConnect(CoreClientPtr client) override;
		virtual void		OnClientDisconnect(CoreClientPtr client) override;
		// P2P Event Handler
		virtual void		OnP2PCreate(P2PClientPtr client) override;
		virtual void		OnMemberJoin(P2PClientPtr client, netid clientId) override;
		virtual void		OnMemberLeave(P2PClientPtr client, netid clientId) override;

	protected:
		CCoreClient	*m_pConnectSvr;			// p2p Server와 연결되는 CoreClient
		CP2PClient	*m_pP2p;
		ClientEventListenerPtr m_pEventListener;

	};

	inline void CClientBasic::SetEventListener(ClientEventListenerPtr ptr) { m_pEventListener = ptr; }
	inline CoreClientPtr CClientBasic::GetConnectSvrClient() const { return CoreClientPtr(m_pConnectSvr); }

}
