
#include "stdafx.h"
#include "Packet.h"

using namespace network;


CPacket::CPacket():
	m_WriteIdx(0)
,	m_ReadIdx(0)
{
}


//------------------------------------------------------------------------
// ��Ŷ�� �������� ������ �����Ѵ�.
//------------------------------------------------------------------------
int	CPacket::GetProtocol() const
{
	const int id = *(int*)m_Data;
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
