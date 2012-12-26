
#include "stdafx.h"
#include "ChatServer.h"


//------------------------------------------------------------------------
// 패킷을 받으면 연결된 모든 클라이언트에게 메세지를 보낸다.
//------------------------------------------------------------------------
void CChatServer::ProcessPacket( const network::CPacket &rcvPacket )
{
// 	if (!IsExist(rcvPacket.GetSenderId()))
// 		return;
// 
// 	// 클라이언트에게 값을 되돌려 줍니다.
// 	char buf[ 256];
// 	strcpy_s(buf, "server send ");
// 	strcat_s(buf, rcvPacket.GetData());
// 	network::CPacket sendPacket(GetSocket(), buf);
// 	SendAll(sendPacket);
}
