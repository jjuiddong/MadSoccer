//------------------------------------------------------------------------
// Name:    UserLobby.h
// Author:  jjuiddong
// Date:    12/29/2012
// 
// �κ� ������ �ִ� ������ �����Ѵ�.
//------------------------------------------------------------------------
#pragma once

DECLARE_TYPE_NAME(CUserLobby)
class CUserLobby : public network::CUser
			, public memmonitor::Monitor<CUserLobby, TYPE_NAME(CUserLobby)>
{
public:
	CUserLobby() {}
	virtual ~CUserLobby() {}

protected:

public:

};
