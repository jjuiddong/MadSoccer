
#include "stdafx.h"
#include "AllProtocol.h"

using namespace network;
using namespace all;


//------------------------------------------------------------------------
// ��� ��Ŷ�� �������ִ� �Լ�
//------------------------------------------------------------------------
void all::Protocol::send(netid targetId, const network::CPacket &packet)
{
	GetNetConnector()->Send(targetId, packet);
}
