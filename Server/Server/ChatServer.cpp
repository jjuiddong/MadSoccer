
#include "stdafx.h"
#include "ChatServer.h"


//------------------------------------------------------------------------
// ��Ŷ�� ������ ����� ��� Ŭ���̾�Ʈ���� �޼����� ������.
//------------------------------------------------------------------------
void CChatServer::ProcessPacket( const network::CPacket &rcvPacket )
{
// 	if (!IsExist(rcvPacket.GetSenderId()))
// 		return;
// 
// 	// Ŭ���̾�Ʈ���� ���� �ǵ��� �ݴϴ�.
// 	char buf[ 256];
// 	strcpy_s(buf, "server send ");
// 	strcat_s(buf, rcvPacket.GetData());
// 	network::CPacket sendPacket(GetSocket(), buf);
// 	SendAll(sendPacket);
}
