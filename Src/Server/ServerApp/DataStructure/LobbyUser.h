//------------------------------------------------------------------------
// Name:    LobbyUser.h
// Author:  jjuiddong
// Date:    12/29/2012
// 
// 로비 서버에 있는 유저를 정의한다.
//------------------------------------------------------------------------
#pragma once

DECLARE_TYPE_NAME(CLobbyUser)
class CLobbyUser : public network::CUser
			, public memmonitor::Monitor<CLobbyUser, TYPE_NAME(CLobbyUser)>
{
public:
	CLobbyUser() {}
	virtual ~CLobbyUser() {}

protected:

public:

};
