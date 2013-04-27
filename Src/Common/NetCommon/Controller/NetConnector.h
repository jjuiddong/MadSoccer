/**
Name:   NetConnector.h
Author:  jjuiddong
Date:    12/25/2012

������ Ŭ���̾�Ʈ�� ���� �θ� Ŭ������. ��������� �ϴ� ��ü���� �Ϲ�ȭ
Ŭ������.
*/
#pragma once

namespace network
{
	class CNetConnector : public CEventHandler, public CSession
	{
	public:
		CNetConnector(PROCESS_TYPE procType);
		virtual ~CNetConnector();

		bool				RegisterProtocol(ProtocolPtr protocol);
		virtual bool	AddProtocolListener(ProtocolListenerPtr pListener);
		virtual bool	RemoveProtocolListener(ProtocolListenerPtr pListener);
		const ProtocolListenerList&	GetProtocolListeners() const;

		//void				ClearConnection();
		//netid			GetNetId() const;
		//SOCKET		GetSocket() const;
		//void				SetSocket(SOCKET sock);

		void				SetParent(NetConnectorPtr parent);
		NetConnectorPtr GetParent() const;
		void				SetThreadHandle(HANDLE handle);
		HANDLE		GetThreadHandle() const;
		PROCESS_TYPE GetProcessType() const;

		// child implementes
		virtual bool	Send(netid netId, const SEND_FLAG flag, const CPacket &packet) = 0;
		virtual bool	SendAll(const CPacket &packet) = 0;

	private:
		//netid						m_Id;
		//SOCKET					m_Socket; 
		NetConnectorPtr		m_pParent;				// CNetConnector ������
		ProtocolListenerList m_ProtocolListeners;		
		HANDLE					m_hThread;				// �Ҽӵ� ������ �ڵ�, ���ٸ� NULL
		PROCESS_TYPE		m_ProcessType;
	};


	//inline void	 CNetConnector::SetSocket(SOCKET sock) { m_Socket = sock; }
	//inline netid CNetConnector::GetNetId() const { return m_Id; }
	//	inline SOCKET CNetConnector::GetSocket() const { return m_Socket; }

	inline const ProtocolListenerList&	 CNetConnector::GetProtocolListeners() const { return m_ProtocolListeners; }
	inline void	 CNetConnector::SetParent(NetConnectorPtr parent) { m_pParent = parent; }
	inline NetConnectorPtr CNetConnector::GetParent() const { return m_pParent; }
	inline void	 CNetConnector::SetThreadHandle(HANDLE handle) { m_hThread = handle; }
	inline HANDLE CNetConnector::GetThreadHandle() const { return m_hThread; }
	inline PROCESS_TYPE CNetConnector::GetProcessType() const { return m_ProcessType; }

}
