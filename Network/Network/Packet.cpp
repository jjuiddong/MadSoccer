
#include "stdafx.h"
#include "Packet.h"

using namespace network;


//------------------------------------------------------------------------
// ��Ŷ�� �������� ������ �����Ѵ�.
//------------------------------------------------------------------------
int	CPacket::GetProtocol() const
{
	const int id = *(int*)m_Data;
	return id;
}
