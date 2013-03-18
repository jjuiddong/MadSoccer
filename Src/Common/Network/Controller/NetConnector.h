//------------------------------------------------------------------------
// Name:    NetConnector.h
// Author:  jjuiddong
// Date:    12/25/2012
// 
// 서버와 클라이언트의 공통 부모 클래스다. 소켓통신을 하는 객체들의 일반화
// 클래스다.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class CNetConnector
	{
	public:
		CNetConnector();
		virtual ~CNetConnector();

		bool				RegisterProtocol(ProtocolPtr protocol);
		const ProtocolListenerList&	GetProtocolListeners() const;
		virtual bool	AddProtocolListener(ProtocolListenerPtr pListener);
		virtual bool	RemoveProtocolListener(ProtocolListenerPtr pListener);
		void				ClearConnection();

		// child implementes
		virtual bool	Send(netid netId, const CPacket &packet) = 0;
		virtual bool	SendAll(const CPacket &packet) = 0;

		netid			GetNetId() const;
		SOCKET		GetSocket() const;

	protected:
		void				SetSocket(SOCKET sock);

	protected:
		netid						m_Id;
		SOCKET					m_Socket; 
		ProtocolListenerList m_ProtocolListeners;
	};

	inline void	 CNetConnector::SetSocket(SOCKET sock) { m_Socket = sock; }
	inline const ProtocolListenerList&	 CNetConnector::GetProtocolListeners() const { return m_ProtocolListeners; }
	inline netid CNetConnector::GetNetId() const { return m_Id; }
	inline SOCKET CNetConnector::GetSocket() const { return m_Socket; }

}
