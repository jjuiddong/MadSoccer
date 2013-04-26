/**
Name:   Session.h
Author:  jjuiddong
Date:    4/26/2013

 Session 
*/
#pragma once


namespace network
{
	
	class CSession
	{
	public:
		CSession();
		virtual ~CSession();

		netid		GetId() const;
		const std::string& GetName();
		SOCKET	GetSocket() const;
		netid		GetGroupId() const;
		P2P_STATE GetP2PState() const;
		CLIENT_STATE GetState() const;
		const std::string& GetIp() const;
		void			SetNetId(netid id) ;
		void			SetName(const std::string &name);
		void			SetSocket(SOCKET sock);
		void			SetP2PState(P2P_STATE state);
		void			SetState(CLIENT_STATE state);
		void			SetIp(const std::string &ip);

	private:
		CLIENT_STATE	m_State;
		netid		m_Id;
		SOCKET	m_Socket;

		P2P_STATE m_P2PState;
		std::string m_Name;
		std::string m_Passwd;
		std::string m_Ip;

	};


	inline netid		CSession::GetId() const { return m_Id; }
	inline const std::string& CSession::GetName() { return m_Name; }
	inline SOCKET	CSession::GetSocket() const { return m_Socket; }
	inline P2P_STATE CSession::GetP2PState() const { return m_P2PState; }
	inline CLIENT_STATE CSession::GetState() const { return m_State; }
	inline const std::string& CSession::GetIp() const { return m_Ip; }
	inline void			CSession::SetNetId(netid id) { m_Id = id; }
	inline void			CSession::SetName(const std::string &name) { m_Name = name; }
	inline void			CSession::SetSocket(SOCKET sock) { m_Socket = sock; }
	inline void			CSession::SetP2PState(P2P_STATE state) { m_P2PState = state; }
	inline void			CSession::SetState(CLIENT_STATE state) { m_State = state; }
	inline void			CSession::SetIp(const std::string &ip) { m_Ip = ip; }

}
