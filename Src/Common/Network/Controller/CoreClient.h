/**
Name:   CoreClient.h
Author:  jjuiddong
Date:    2/28/2013

CServer 에 접속하는 클라이언트 클래스다. 
*/
#pragma once


namespace network
{
	class CCoreClient : public CNetConnector
	{
		friend class CNetLauncher;
		friend class CNetController;
		friend class CClientBasic;
		friend class CP2PClient;

	public:
		CCoreClient(PROCESS_TYPE procType);
		virtual ~CCoreClient();

		bool				Stop();
		void				Disconnect();
		void				Close();

		bool				IsConnect() const;
		void				SetConnect(bool isConnect);
		void				SetServerIp(const std::string &ip);
		void				SetServerPort(int port);
		const std::string& GetServerIp() const;
		int				GetServerPort() const;

		virtual bool	Send(netid netId, const SEND_FLAG flag, const CPacket &packet);
		virtual bool	SendAll(const CPacket &packet) override;

		// Event Handler
		void				OnConnect();
		void				OnDisconnect();
		void				OnMemberJoin(netid netId); /// P2P member join
		void				OnMemberLeave(netid netId); /// P2P member leave

	protected:
		bool				Proc();
		void				DispatchPacket();
		void				Clear();

	private:
		std::string	m_ServerIP;
		int				m_ServerPort;
		bool				m_IsConnect;

	};


	inline bool CCoreClient::IsConnect() const { return m_IsConnect; }
	inline void	 CCoreClient::SetConnect(bool isConnect) { m_IsConnect = isConnect; }
	inline void	 CCoreClient::SetServerIp(const std::string &ip) { m_ServerIP = ip; }
	inline void	 CCoreClient::SetServerPort(int port) { m_ServerPort = port; }
	inline const std::string& CCoreClient::GetServerIp() const { return m_ServerIP; }
	inline int CCoreClient::GetServerPort() const { return m_ServerPort; }

}
