
#include "stdafx.h"
#include "AllProtocol.h"

using namespace network;
using namespace all;


//------------------------------------------------------------------------
// ��� ��Ŷ�� �������ִ� �Լ�
//------------------------------------------------------------------------
void all::Protocol::send(netid targetId, const network::SEND_FLAG flag, const network::CPacket &packet)
{
	GetNetConnector()->Send(targetId, flag, packet);
}
