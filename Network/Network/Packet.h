//------------------------------------------------------------------------
// Name:    Packet.h
// Author:  jjuiddong
// Date:    2012-11-28
// 
// ��Ʈ��ũ ������� ���� ��Ŷ�� �����Ѵ�.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class CPacket
	{
	public:
		enum { MAX_PACKETSIZE = 256, };

		CPacket() {}
		CPacket(SOCKET sender, char *buf256) 
		{
			m_Sender = sender;
			if (buf256)
				memcpy( m_Data, buf256, MAX_PACKETSIZE);
		}
		virtual ~CPacket() {}

	protected:
		SOCKET		m_Sender;					// ��Ŷ�� ���� ���� 
		char		m_Data[ MAX_PACKETSIZE];

	public:
		int			GetProtocol() const;
		SOCKET		GetSenderSocket() const { return m_Sender; }
		char*		GetData() const { return (char*)m_Data; }
		int			GetPacketSize() const { return MAX_PACKETSIZE; }

	};
}
