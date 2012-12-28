//------------------------------------------------------------------------
// Name:    Packet.h
// Author:  jjuiddong
// Date:    2012-11-28
// 
// 네트워크 통신에 쓰이는 패킷을 정의한다.
//------------------------------------------------------------------------
#pragma once

#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_array.hpp>

namespace network
{
	class CPacket
	{
	public:
		enum { 
			MAX_PACKETSIZE = 256, 
			MAX_STRINGSIZE = 128, //최대 스트링 사이즈
		};

		CPacket();
		CPacket(netid senderId, char *buf256);
		virtual ~CPacket() {}

	protected:
		netid		m_SenderId;
		char		m_Data[ MAX_PACKETSIZE];
		int			m_ReadIdx;
		int			m_WriteIdx;

	public:
		int			GetProtocolId() const;
		int			GetPacketId() const;
		netid		GetSenderId() const { return m_SenderId; }
		char*		GetData() const { return (char*)m_Data; }
		int			GetPacketSize() const { return m_WriteIdx; }

		CPacket& operator<<(const bool &rhs);
		CPacket& operator<<(const int &rhs);
		CPacket& operator<<(const char &rhs);
		CPacket& operator<<(const float &rhs);
		CPacket& operator<<(const double &rhs);
		CPacket& operator<<(const short &rhs);
		CPacket& operator<<(const std::string &rhs);

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
		CPacket& operator>>(std::string &rhs);

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
		// 패킷의 m_WriteIdx부터 데이타를 저장한다.
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
		// 패킷의 m_ReadIdx 부터 데이타를 가져온다.
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
