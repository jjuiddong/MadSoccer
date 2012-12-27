//------------------------------------------------------------------------
// Name:    NetConnector.h
// Author:  jjuiddong
// Date:    12/25/2012
// 
// ������ Ŭ���̾�Ʈ�� ���� �θ� Ŭ������. ��������� �ϴ� ��ü���� �Ϲ�ȭ
// Ŭ������.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class CNetConnector
	{
	public:
		CNetConnector();
		virtual ~CNetConnector();
		friend class CNetLauncher;

	protected:
		netid				m_NetId;				// ����ID (�ڵ�����)
		SOCKET				m_Socket;
		ProtocolListenerMap	m_ProtocolListeners;

	public:
		netid				GetNetId() const { return m_NetId; }
		SOCKET				GetSocket() const { return m_Socket; }

		bool				RegisterProtocol(ProtocolPtr protocol);
		bool				AddListener(ProtocolListenerPtr pListener);
		bool				RemoveListener(ProtocolListenerPtr pListener);
		const ProtocolListenerList&	GetListeners(int listenerId);
		bool				IsExistListener(int listenerId);

		// child implementes
		virtual bool		Send(netid netId, const CPacket &packet) = 0;
		virtual bool		SendAll(const CPacket &packet) = 0;

	protected:
		void				SetSocket(SOCKET sock) { m_Socket = sock; }

	};
}
