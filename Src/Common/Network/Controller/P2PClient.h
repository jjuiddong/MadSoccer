/**
Name:   P2PClient.h
Author:  jjuiddong
Date:    2/28/2013

P2P 통신을 하는 클래스다.
*/
#pragma once

#include "../interface/CoreClientEventListener.h"
#include "../interface/ServerEventListener.h"
#include "../interface/P2PClientEventListener.h"

namespace network
{
	class CServerBasic;
	class CCoreClient;

	DECLARE_TYPE_NAME_SCOPE(network, CP2PClient)
	class CP2PClient : public CNetConnector
								, public ICoreClientEventListener
								, public IServerEventListener
								, public memmonitor::Monitor<CP2PClient, TYPE_NAME(CP2PClient)>
	{
		friend class CNetController;
		friend class CClient;
		typedef common::ReferencePtr<IP2PClientEventListener> P2PClientEventListenerPtr;

	public:
		CP2PClient(PROCESS_TYPE procType);
		virtual ~CP2PClient();

		bool				Bind( const int port );
		bool				Connect( const std::string &ip, const int port  );
		bool				Proc();
		bool				Stop();
		void				Disconnect();
		void				Close();
		void				SetEventListener(P2PClientEventListenerPtr ptr);

		bool				IsConnect() const;

		// Overriding
		virtual bool	AddProtocolListener(ProtocolListenerPtr pListener) override;
		virtual bool	RemoveProtocolListener(ProtocolListenerPtr pListener) override;

		// Child Implementes
		virtual bool	Send(netid netId, const SEND_FLAG flag, const CPacket &packet) override;
		virtual bool	SendAll(const CPacket &packet) override;

	protected:
		void				Clear();
		bool				CreateP2PHost(const int port);
		bool				CreateP2PClient(const std::string &ip, const int port);

		// P2P Client Event Handler
		virtual void	OnCoreClientConnect(CoreClientPtr client) override;
		virtual void	OnClientDisconnect(CoreClientPtr client) override;

		// P2P Host Event Handler;
		virtual void	OnListen(ServerBasicPtr svr) override;
		virtual void	OnServerDisconnect(ServerBasicPtr svr) override;
		virtual void	OnClientJoin(ServerBasicPtr svr, netid netId) override;
		virtual void	OnClientLeave(ServerBasicPtr svr, netid netId) override;

	protected:
		P2P_STATE		m_State;
		CCoreClient		*m_pP2pClient;
		CServerBasic	*m_pP2pHost;
		P2PClientEventListenerPtr m_pEventListener;
	};

	inline bool CP2PClient::IsConnect() const { return true; }
	inline void	 CP2PClient::SetEventListener(P2PClientEventListenerPtr ptr) { m_pEventListener = ptr; }

}
