
#include "stdafx.h"
#include "Packet.h"

using namespace network;


//------------------------------------------------------------------------
// 패킷의 프로토콜 정보를 리턴한다.
//------------------------------------------------------------------------
int	CPacket::GetProtocol() const
{
	const int id = *(int*)m_Data;
	return id;
}
