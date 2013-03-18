/**
Name:   ClientBasic.h
Author:  jjuiddong
Date:    3/17/2013

	CServer�� �����ϴ� Client�� ǥ���Ѵ�.
	P2P ��� ���
*/
#pragma once

#include "Controller/NetConnector.h"
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

		// Overriding
		virtual bool		AddProtocolListener(ProtocolListenerPtr pListener) override;
		virtual bool		RemoveProtocolListener(ProtocolListenerPtr pListener) override;
		virtual bool		Send(netid netId, const CPacket &packet) override;
		virtual bool		SendAll(const CPacket &packet) override;

		bool					IsConnect() const;
		void					SetEventListener(ClientEventListenerPtr ptr);
		PROCESS_TYPE GetProcessType() const;
		CoreClientPtr	GetConnectSvrClient() const;

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
		PROCESS_TYPE m_ProcessType;		// m_pConnectSvr �� ���� ����.
		CCoreClient	*m_pConnectSvr;			// p2p Server�� ����Ǵ� CoreClient
		CP2PClient	*m_pP2p;
		ClientEventListenerPtr m_pEventListener;

	};

	inline void CClientBasic::SetEventListener(ClientEventListenerPtr ptr) { m_pEventListener = ptr; }
	inline PROCESS_TYPE CClientBasic::GetProcessType() const { return m_ProcessType; }
	inline CoreClientPtr CClientBasic::GetConnectSvrClient() const { return CoreClientPtr(m_pConnectSvr); }

}