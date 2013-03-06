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

		netid			GetNetId() const { return m_NetId; }
		SOCKET		GetSocket() const { return m_Socket; }

		bool				RegisterProtocol(ProtocolPtr protocol);
		const ProtocolListenerList&	GetListeners() { return m_ProtocolListeners; }
		virtual bool	AddListener(ProtocolListenerPtr pListener);
		virtual bool	RemoveListener(ProtocolListenerPtr pListener);
		void				ClearConnection();

		// child implementes
		virtual bool		Send(netid netId, const CPacket &packet) = 0;
		virtual bool		SendAll(const CPacket &packet) = 0;

	protected:
		void				SetSocket(SOCKET sock) { m_Socket = sock; }

	protected:
		netid						m_NetId;				// 고유ID (자동생성)
		SOCKET					m_Socket; 
		ProtocolListenerList m_ProtocolListeners;
	};
}
