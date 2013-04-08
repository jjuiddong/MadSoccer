/**
Name:   RemoteClient.h
Author:  jjuiddong
Date:    12/25/2012

������ ���ӵ� Ŭ���̾�Ʈ ������ ������.
*/
#pragma once

namespace network
{
	//DECLARE_TYPE_NAME_SCOPE(network, CRemoteClient)
	class CRemoteClient 
		//: public memmonitor::Monitor<CRemoteClient, TYPE_NAME(network::CRemoteClient)>
	{
	public:
		CRemoteClient();
		virtual ~CRemoteClient();

		netid		GetId() const;
		SOCKET	GetSocket() const;
		netid		GetGroupId() const;
		P2P_STATE GetP2PState() const;
		const std::string& GetIp() const;
		void			SetNetId(netid id) ;
		void			SetSocket(SOCKET sock);
		void			SetP2PState(P2P_STATE state);
		void			SetIp(const std::string &ip);

	private:
		netid		m_Id;
		SOCKET	m_Socket;
		P2P_STATE m_P2PState;
		std::string m_Ip;
	};

	inline netid		CRemoteClient::GetId() const { return m_Id; }
	inline SOCKET	CRemoteClient::GetSocket() const { return m_Socket; }
	inline P2P_STATE CRemoteClient::GetP2PState() const { return m_P2PState; }
	inline const std::string& CRemoteClient::GetIp() const { return m_Ip; }
	inline void			CRemoteClient::SetNetId(netid id) { m_Id = id; }
	inline void			CRemoteClient::SetSocket(SOCKET sock) { m_Socket = sock; }
	inline void			CRemoteClient::SetP2PState(P2P_STATE state) { m_P2PState = state; }
	inline void			CRemoteClient::SetIp(const std::string &ip) { m_Ip = ip; }

}
