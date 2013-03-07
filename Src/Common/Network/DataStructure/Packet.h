//------------------------------------------------------------------------
// Name:    Packet.h
// Author:  jjuiddong
// Date:    2012-11-28
// 
// ��Ʈ��ũ ��ſ� ���̴� ��Ŷ�� �����Ѵ�.
//
// ������ PacketId
// 0 : Connect
// 1 : Disconnect
//------------------------------------------------------------------------
#pragma once

#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_array.hpp>

namespace network
{
	enum RESERVED_PACKETID
	{
		PACKETID_CONNECT = 0,
		PACKETID_DISCONNECT,
	};

	class CPacket
	{
	public:
		enum 
		{ 
			MAX_PACKETSIZE = 256, 
			MAX_STRINGSIZE = 128, //�ִ� ��Ʈ�� ������
		};

		CPacket();
		CPacket(netid senderId, char *buf256);
		virtual ~CPacket() {}

		int			GetProtocolId() const;
		int			GetPacketId() const;
		netid		GetSenderId() const { return m_SenderId; }
		char*		GetData() const { return (char*)m_Data; }
		int			GetPacketSize() const { return m_WriteIdx; }

		template<class T>
		CPacket& operator<<(const T& rhs)
		{
			//const bool isPointer = boost::is_pointer<T>::type::value;
			//assert(!isPointer);
			//const bool isArray = boost::is_array<T>::type::value;
			//if (isArray)	AppendPtr(rhs, sizeof(T));
			//else			Append(rhs);
			Append(rhs);
			return *this;
		}

		template<class T>
		CPacket& operator>>(T& rhs)
		{
			//const bool isPointer = boost::is_pointer<T>::type::value;
			//assert(!isPointer);
			//const bool isArray = boost::is_array<T>::type::value;
			//if (isArray)	GetDataPtr(rhs, sizeof(T));
			//else			GetData(rhs);
			GetData(rhs);
			return *this;
		}

		//------------------------------------------------------------------------
		// ��Ŷ�� m_WriteIdx���� ����Ÿ�� �����Ѵ�.
		//------------------------------------------------------------------------
		template<class T> void Append(const T &rhs)
		{
			if (m_WriteIdx+sizeof(T) >= MAX_PACKETSIZE)
				return;
			memmove_s(m_Data+m_WriteIdx, MAX_PACKETSIZE-m_WriteIdx, &rhs, sizeof(T));
			m_WriteIdx += sizeof(T);
		}
		template<class T> void AppendPtr(const T &rhs, const size_t size) // size : copy byte size
		{
			if (m_WriteIdx+size >= MAX_PACKETSIZE)
				return;
			memmove_s(m_Data+m_WriteIdx, MAX_PACKETSIZE-m_WriteIdx, rhs, size);
			m_WriteIdx += size;
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
		template<class T> void GetDataPtr(T &rhs, size_t size)
		{
			if (m_ReadIdx+size >= MAX_PACKETSIZE)
				return;
			memmove_s(rhs, size, m_Data+m_ReadIdx, size);
			m_ReadIdx += size;
		}
		void GetDataString( std::string &str)
		{
			char buf[ CPacket::MAX_STRINGSIZE] = {NULL,};
			for (int i=0; i < MAX_STRINGSIZE-1 && (m_ReadIdx < MAX_PACKETSIZE); ++i)
			{
				buf[ i] = m_Data[ m_ReadIdx++];
				if (NULL == m_Data[ m_ReadIdx-1])
					break;
			}
			str = buf;
		}
		//_variant_t GetVariant(const _variant_t &varType);

	protected:
		netid		m_SenderId;
		char			m_Data[ MAX_PACKETSIZE];
		int			m_ReadIdx;
		int			m_WriteIdx;

	};


	// Global Functions
	CPacket DisconnectPacket();

}
