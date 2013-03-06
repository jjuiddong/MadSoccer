//------------------------------------------------------------------------
// Name:    Client.h
// Author:  jjuiddong
// Date:    2012-11-27
//
// CServer�� �����ϴ� Client�� ǥ���Ѵ�.
// P2P ��� ���
//------------------------------------------------------------------------
#pragma once

#include "Controller/NetConnector.h"
#include "../interface/CoreClientEventListener.h"
#include "../interface/P2PClientEventListener.h"
#include "../interface/ClientEventListener.h"

namespace network
{
	class CP2PClient;
	class CCoreClient;
	class CClient : public CNetConnector
							,public ICoreClientEventListener
							,public IP2PClientEventListener
	{
		friend class CNetLauncher;
		friend class CNetController;
		typedef common::ReferencePtr<IClientEventListener> ClientEventListenerPtr;

	public:
		CClient(PROCESS_TYPE procType);
		virtual ~CClient();
		PROCESS_TYPE GetProcessType() const { return m_ProcessType; }
		CoreClientPtr	GetConnectSvrClient() const { return CoreClientPtr(m_pConnectSvr); }
		bool					IsConnect() const;
		void					SetEventListener(ClientEventListenerPtr ptr) { m_pEventListener = ptr; }

		bool					Proc();
		bool					Stop();
		void					Disconnect();

		// Overriding
		virtual bool		AddListener(ProtocolListenerPtr pListener) override;
		virtual bool		RemoveListener(ProtocolListenerPtr pListener) override;
		virtual bool		Send(netid netId, const CPacket &packet) override;
		virtual bool		SendAll(const CPacket &packet) override;

	protected:
		void					Clear();

		// Event Handler
		virtual void		OnCoreClientConnect(CoreClientPtr client) override;
		virtual void		OnClientDisconnect(CoreClientPtr client) override;
		//virtual void		OnMemberJoin(CoreClientPtr client) override;
		//virtual void		OnMemberLeave(CoreClientPtr client) override;

	protected:
		PROCESS_TYPE m_ProcessType;		// m_pConnectSvr �� ���� ����.
		CCoreClient	*m_pConnectSvr;			// p2p Server�� ����Ǵ� CoreClient
		CP2PClient	*m_pP2p;
		ClientEventListenerPtr m_pEventListener;		

	};
};
