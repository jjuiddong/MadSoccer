//------------------------------------------------------------------------
// Name:    LobbyPlayer.h
// Author:  jjuiddong
// Date:    12/29/2012
// 
// 로비 서버에 있는 유저를 정의한다.
//------------------------------------------------------------------------
#pragma once

DECLARE_TYPE_NAME(CLobbyPlayer)
class CLobbyPlayer : public network::CPlayer
			, public memmonitor::Monitor<CLobbyPlayer, TYPE_NAME(CLobbyPlayer)>
{
public:
	CLobbyPlayer() {}
	virtual ~CLobbyPlayer() {}

protected:

public:

};
