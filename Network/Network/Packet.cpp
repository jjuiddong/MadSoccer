
#include "stdafx.h"
#include "Packet.h"

using namespace network;


CPacket::CPacket():
	m_WriteIdx(0)
,	m_ReadIdx(0)
{
}

CPacket::CPacket(netid senderId, char *buf256) :
	m_SenderId(senderId)
,	m_WriteIdx(0)
,	m_ReadIdx(0)
{
	if (buf256)
		memcpy( m_Data, buf256, MAX_PACKETSIZE);
}


//------------------------------------------------------------------------
// ��Ŷ�� �������� ������ �����Ѵ�.
//------------------------------------------------------------------------
int	CPacket::GetProtocolId() const
{
	const int id = *(int*)m_Data;
	return id;
}


//------------------------------------------------------------------------
// ��Ŷ�� ��Ŷ ���̵� ������ �����Ѵ�.
//------------------------------------------------------------------------
int	CPacket::GetPacketId() const 
{
	const int id = *(int*)(m_Data+sizeof(int));
	return id;
}


//------------------------------------------------------------------------
// ����Ÿ �Ҵ�
//------------------------------------------------------------------------
CPacket& CPacket::operator<<(const bool &rhs)
{
	Append(rhs);
	return *this;
}
CPacket& CPacket::operator<<(const int &rhs)
{
	Append(rhs);
	return *this;
}
CPacket& CPacket::operator<<(const char &rhs)
{
	Append(rhs);
	return *this;
}
CPacket& CPacket::operator<<(const float &rhs)
{
	Append(rhs);
	return *this;
}
CPacket& CPacket::operator<<(const double &rhs)
{
	Append(rhs);
	return *this;
}
CPacket& CPacket::operator<<(const short &rhs)
{
	Append(rhs);
	return *this;
}
CPacket& CPacket::operator<<(const std::string &rhs)
{
	const int length = rhs.size();
	if (m_WriteIdx+length+1 >= MAX_PACKETSIZE)
		return *this;

	memcpy_s(m_Data+m_WriteIdx, MAX_PACKETSIZE-m_WriteIdx, rhs.c_str(), length);
	m_WriteIdx += length;
	m_Data[ m_WriteIdx] = NULL;
	++m_WriteIdx;
	return *this;
}


//------------------------------------------------------------------------
// ����Ÿ �б�
//------------------------------------------------------------------------
CPacket& CPacket::operator>>(bool &rhs)
{
	GetData(rhs);
	return *this;
}
CPacket& CPacket::operator>>(int &rhs)
{
	GetData(rhs);
	return *this;
}
CPacket& CPacket::operator>>(char &rhs)
{
	GetData(rhs);
	return *this;
}
CPacket& CPacket::operator>>(float &rhs)
{
	GetData(rhs);
	return *this;
}
CPacket& CPacket::operator>>(double &rhs)
{
	GetData(rhs);
	return *this;
}
CPacket& CPacket::operator>>(short &rhs)
{
	GetData(rhs);
	return *this;
}
CPacket& CPacket::operator>>(std::string &rhs)
{
	char buf[ MAX_STRINGSIZE] = {NULL,};
	for (int i=0; i < MAX_STRINGSIZE-1 && (m_ReadIdx < MAX_PACKETSIZE); ++i)
	{
		buf[ i] = m_Data[ m_ReadIdx++];
		if (NULL == m_Data[ m_ReadIdx-1])
			break;
	}
	rhs = buf;
	return *this;
}
