/**
Name:   NetConnectorLinker.h
Author:  jjuiddong
Date:    4/3/2013

	network::CNetConnectorLinker 구현

	- CNetConnectorLinker 는 CNetConnector 클래스를 이어주는 역할을 한다.
	- CNetConnector Wraping 클래스

	- CNetConnector 객체는 아니면서, CNetConnector 객체와 패킷을 주고 받는 객체는 이 클래스를
	상속받아 구현하면 간편하다.
*/
#pragma once

#include "NetConnector.h"

namespace network
{

	/// NetConnector Wrapper class, CEventHandler 를 protected 상속을 받아서 Event에 관련된
	/// 함수에 접근하지 못하도록 한다. 기본적으로 CNetConnectorLinker의 Event 는 GetConnector()
	/// 함수를 통해 등록되어야 한다.
	class CNetConnectorLinker : protected CEventHandler
	{
	public:
		CNetConnectorLinker();
		CNetConnectorLinker(NetConnectorPtr ptr);
		virtual ~CNetConnectorLinker() { }

		void				SetNetConnector( NetConnectorPtr ptr );
		NetConnectorPtr GetNetConnector() const;
		bool				RegisterProtocol(ProtocolPtr protocol);
		virtual bool	AddProtocolListener(ProtocolListenerPtr pListener);
		virtual bool	RemoveProtocolListener(ProtocolListenerPtr pListener);
		const ProtocolListenerList&	GetProtocolListeners() const;
		void				ClearConnection();

		using CEventHandler::EventConnect;
		bool				EventConnect(CEventHandler *handler, EventType type, EventFunction fn);

		netid			GetNetId() const;
		SOCKET		GetSocket() const;
		void				SetSocket(SOCKET sock);
		void				SetParent(NetConnectorPtr parent);
		NetConnectorPtr GetParent() const;
		void				SetThreadHandle(HANDLE handle);
		HANDLE		GetThreadHandle() const;
		PROCESS_TYPE GetProcessType() const;

		// child implementes
		virtual bool	Send(netid netId, const SEND_FLAG flag, const CPacket &packet);
		virtual bool	SendAll(const CPacket &packet);

	private:
		NetConnectorPtr m_pNetCon;
		static ProtocolListenerList s_emptyListeners;
	};


	ProtocolListenerList CNetConnectorLinker::s_emptyListeners;
	inline CNetConnectorLinker::CNetConnectorLinker() : m_pNetCon(NULL) {}
	inline CNetConnectorLinker::CNetConnectorLinker(NetConnectorPtr ptr) : m_pNetCon(ptr) {}
	inline void	CNetConnectorLinker::SetNetConnector( NetConnectorPtr ptr ) { m_pNetCon = ptr; }
	inline NetConnectorPtr CNetConnectorLinker::GetNetConnector() const { return m_pNetCon; }
	inline void	 CNetConnectorLinker::SetSocket(SOCKET sock) { if (m_pNetCon) m_pNetCon->SetSocket(sock); }

	inline bool CNetConnectorLinker::RegisterProtocol(ProtocolPtr protocol) { if (m_pNetCon) return m_pNetCon->RegisterProtocol(protocol); return false; }
	inline bool CNetConnectorLinker::AddProtocolListener(ProtocolListenerPtr pListener) { if (m_pNetCon) return m_pNetCon->AddProtocolListener(pListener); return false; }
	inline bool CNetConnectorLinker::RemoveProtocolListener(ProtocolListenerPtr pListener) { if (m_pNetCon) return m_pNetCon->RemoveProtocolListener(pListener); return false;}

	inline const ProtocolListenerList&	 CNetConnectorLinker::GetProtocolListeners() const { return (m_pNetCon!=NULL? m_pNetCon->GetProtocolListeners() : s_emptyListeners); }
	inline netid CNetConnectorLinker::GetNetId() const { return (m_pNetCon!=NULL? m_pNetCon->GetNetId() : INVALID_NETID); }
	inline SOCKET CNetConnectorLinker::GetSocket() const { return (m_pNetCon!=NULL? m_pNetCon->GetSocket() : 0); }
	inline void	 CNetConnectorLinker::SetParent(NetConnectorPtr parent) { if (m_pNetCon!=NULL) m_pNetCon->SetParent(parent); }
	inline NetConnectorPtr CNetConnectorLinker::GetParent() const { return (m_pNetCon!=NULL? m_pNetCon->GetParent() : NULL); }
	inline void	 CNetConnectorLinker::SetThreadHandle(HANDLE handle) { if (m_pNetCon!=NULL) m_pNetCon->SetThreadHandle(handle); }
	inline HANDLE CNetConnectorLinker::GetThreadHandle() const { return (m_pNetCon!=NULL? m_pNetCon->GetThreadHandle() : NULL); }
	inline PROCESS_TYPE CNetConnectorLinker::GetProcessType() const { return (m_pNetCon!=NULL? m_pNetCon->GetProcessType() : USER_LOOP); }
	inline bool CNetConnectorLinker::EventConnect(CEventHandler *handler, EventType type, EventFunction fn) {
		if (m_pNetCon)
			return m_pNetCon->EventConnect(handler, type, fn);
		return false;
	}
	inline bool CNetConnectorLinker::Send(netid netId, const SEND_FLAG flag, const CPacket &packet) {
		if (m_pNetCon)
			return m_pNetCon->Send(netId, flag, packet);
		return false;
	}
	inline bool CNetConnectorLinker::SendAll(const CPacket &packet) {
		if (m_pNetCon)
			return m_pNetCon->SendAll(packet);
		return false;
	}

}
