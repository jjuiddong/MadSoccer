//------------------------------------------------------------------------
// Name:    Packet.h
// Author:  jjuiddong
// Date:    2012-11-28
// 
// ��Ʈ��ũ ������� ���� ��Ŷ�� �����Ѵ�.
//------------------------------------------------------------------------
#pragma once

#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_array.hpp>

namespace network
{
	class CPacket
	{
	public:
		enum { MAX_PACKETSIZE = 256, };

		CPacket();
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
		int			m_ReadIdx;
		int			m_WriteIdx;

	public:
		int			GetProtocol() const;
		SOCKET		GetSenderSocket() const { return m_Sender; }
		char*		GetData() const { return (char*)m_Data; }
		int			GetPacketSize() const { return m_WriteIdx; }

		CPacket& operator<<(const bool &rhs);
		CPacket& operator<<(const int &rhs);
		CPacket& operator<<(const char &rhs);
		CPacket& operator<<(const float &rhs);
		CPacket& operator<<(const double &rhs);
		CPacket& operator<<(const short &rhs);

		template<class T>
		CPacket& operator<<(const T& rhs)
		{
			const bool isPointer = boost::is_pointer<T>::type::value;
			assert(!isPointer);
			const bool isArray = boost::is_array<T>::type::value;
			if (isArray)	AppendPtr(rhs);
			else			Append(rhs);
			return *this;
		}

		CPacket& operator>>(bool &rhs);
		CPacket& operator>>(int &rhs);
		CPacket& operator>>(char &rhs);
		CPacket& operator>>(float &rhs);
		CPacket& operator>>(double &rhs);
		CPacket& operator>>(short &rhs);

		template<class T>
		CPacket& operator>>(T& rhs)
		{
			const bool isPointer = boost::is_pointer<T>::type::value;
			assert(!isPointer);
			const bool isArray = boost::is_array<T>::type::value;
			if (isArray)	GetDataPtr(rhs);
			else			GetData(rhs);
			return *this;
		}

		//------------------------------------------------------------------------
		// ��Ŷ�� �������� ����Ÿ�� �����Ѵ�.
		//------------------------------------------------------------------------
		template<class T> void Append(const T &rhs)
		{
			if (m_WriteIdx+sizeof(T) >= MAX_PACKETSIZE)
				return;
			memmove_s(m_Data+m_WriteIdx, MAX_PACKETSIZE-m_WriteIdx, &rhs, sizeof(T));
			m_WriteIdx += sizeof(T);
		}
		template<class T> void AppendPtr(const T &rhs)
		{
			if (m_WriteIdx+sizeof(T) >= MAX_PACKETSIZE)
				return;
			memmove_s(m_Data+m_WriteIdx, MAX_PACKETSIZE-m_WriteIdx, rhs, sizeof(T));
			m_WriteIdx += sizeof(T);
		}

		//------------------------------------------------------------------------
		// ��Ŷ�� m_ReadIdx ���� ����Ÿ�� �����´�.
		//------------------------------------------------------------------------
		template<class T> void GetData(T &rhs)
		{
			if (m_ReadIdx+sizeof(T) >= MAX_PACKETSIZE)
				return;
			memmove_s(&rhs, sizeof(T), m_Data+m_ReadIdx, sizeof(T));
			m_ReadIdx += sizeof(T);
		}
		template<class T> void GetDataPtr(T &rhs)
		{
			if (m_ReadIdx+sizeof(T) >= MAX_PACKETSIZE)
				return;
			memmove_s(rhs, sizeof(T), m_Data+m_ReadIdx, sizeof(T));
			m_ReadIdx += sizeof(T);
		}
	};
}
