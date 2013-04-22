/**
Name:   NetGroupController.h
Author:  jjuiddong
Date:    4/1/2013

하나 이상의 서버군들에게 접속하는 클래스다.
	Server/Clients/P2p 를 관리한다.

	Propagate NetEvent
		- EVT_CONNECT
		- EVT_DISCONNECT
*/
#pragma once

namespace network { namespace multinetwork {

	///  네트워크 그룹에 접근하는 Server/Client/P2p 를 관리하는 클래스.
	DECLARE_TYPE_NAME_SCOPE(network::multinetwork, CNetGroupController)
	class CNetGroupController : public CNetConnector
												, public memmonitor::Monitor<CNetGroupController, TYPE_NAME(network::multinetwork::CNetGroupController)>
	{
	public:
		enum STATE {
			WAIT,
			TRYCONNECT, // try connect
			RUN, // after connect
			END, // after call Stop()
		};

		CNetGroupController( SERVICE_TYPE type, const std::string &svrType, 
			const std::string &connectSvrType );
		virtual ~CNetGroupController();

		bool				Start(const std::string &ip, const int port);
		bool				Start(const std::vector<SHostInfo> &v);
		void				Stop();

		virtual bool	Send(netid netId, const SEND_FLAG flag, const CPacket &packet) override;
		virtual bool	SendAll(const CPacket &packet) override;

		const std::string& GetSvrType() const;
		const std::string& GetConnectSvrType() const;
		void				SetRemoteClientFactory( IRemoteClientFactory *ptr );
		void				SetGroupFactory( IGroupFactory *ptr );
		SERVICE_TYPE GetServiceType() const;
		bool				IsConnect() const;
		bool				IsTryConnect() const;
		void				SetTryConnect();
		CServerBasic* GetServer();
		CCoreClient* GetClient();

		// overriding
		virtual bool	AddProtocolListener(ProtocolListenerPtr pListener) override;
		virtual bool	RemoveProtocolListener(ProtocolListenerPtr pListener) override;

	protected:
		bool				Connect( SERVICE_TYPE type, const std::string &ip, const int port );

	private:
		// Event Handler
		void				OnConnect( CNetEvent &event );
		void				OnDisconnect( CNetEvent &event );

	private:
		typedef common::VectorMap<netid, CCoreClient*> Clients;

		STATE m_State;
		SERVICE_TYPE m_ServiceType;
		std::string m_svrType;
		std::string m_connectSvrType;
		std::string m_Ip;
		int m_Port;

		CCoreClient *m_pClient;
		CServerBasic *m_pServer;
		CP2PClient *m_pP2p;
		Clients m_Clients;
		Clients m_RemoveClients;
		IRemoteClientFactory *m_pRemoteClientFactory;
		IGroupFactory *m_pGroupFactory;
		
	};


	inline const std::string& CNetGroupController::GetSvrType() const { return m_svrType; }
	inline const std::string& CNetGroupController::GetConnectSvrType() const { return m_connectSvrType; }
	inline SERVICE_TYPE CNetGroupController::GetServiceType() const { return m_ServiceType; }
	inline CServerBasic* CNetGroupController::GetServer() { return m_pServer; }
	inline CCoreClient* CNetGroupController::GetClient() { return m_pClient; }
	inline bool CNetGroupController::IsConnect() const { return (m_State == RUN) || (m_State == TRYCONNECT); }
	inline bool CNetGroupController::IsTryConnect() const { return (m_State == TRYCONNECT); }
	inline void	CNetGroupController::SetTryConnect() { m_State = TRYCONNECT; }

}}
