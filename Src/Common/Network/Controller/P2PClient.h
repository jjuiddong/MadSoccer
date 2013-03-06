//------------------------------------------------------------------------
// Name:    P2PClient.h
// Author:  jjuiddong
// Date:    2/28/2013
// 
// P2P ����� �ϴ� Ŭ������.
//------------------------------------------------------------------------
#pragma once

#include "NetConnector.h"
#include "../interface/CoreClientEventListener.h"
#include "../interface/ServerEventListener.h"

namespace network
{
	class CServer;
	class CCoreClient;
	DECLARE_TYPE_NAME(CP2PClient)
	class CP2PClient : public CNetConnector
								, public ICoreClientEventListener
								, public IServerEventListener
								, public sharedmemory::CSharedMem<CP2PClient, TYPE_NAME(CP2PClient)>
	{
		friend class CNetController;
		friend class CClient;

	public:
		enum STATE
		{
			Host,		// P2P Host
			Client,		// P2P Client
		};

		CP2PClient(PROCESS_TYPE procType);
		virtual ~CP2PClient();
		PROCESS_TYPE GetProcessType() const { return m_ProcessType; }
		bool				Connect( STATE state, const int port, const std::string &ip="" );
		bool				IsConnect() const { return true; }

		bool				Proc();
		bool				Stop();
		void				Disconnect();

		// child implementes
		virtual bool	Send(netid netId, const CPacket &packet);
		virtual bool	SendAll(const CPacket &packet);

	protected:
		void				Clear();
		bool				CreateP2PHost(const int port);
		bool				CreateP2PClient(const std::string &ip, const int port);

		// Event Handler
		void				OnMemberJoin();
		void				OnMemberLeave();

	protected:
		STATE			m_State;
		PROCESS_TYPE m_ProcessType;
		CCoreClient	*m_pP2pClient;
		CServer		*m_pP2pHost;
	};
}
