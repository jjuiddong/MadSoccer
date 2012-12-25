//------------------------------------------------------------------------
// Name:    ChatServer.h
// Author:  jjuiddong
// Date:    12/21/2012
// 
// 채팅서버
//------------------------------------------------------------------------

#pragma once


class CChatServer : public network::CServer
{
protected:
	virtual void ProcessPacket( const network::CPacket &rcvPacket );

};

