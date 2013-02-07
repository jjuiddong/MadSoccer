//------------------------------------------------------------------------
// Name:    RoomLobby.h
// Author:  jjuiddong
// Date:    12/29/2012
// 
// 로비서버에 존재하는 방정보를 정의한다.
//------------------------------------------------------------------------
#pragma once

DECLARE_TYPE_NAME(CRoomLobby)
class CRoomLobby : public network::CRoom
			, public sharedmemory::CSharedMem<CRoomLobby, TYPE_NAME(CRoomLobby)>
{
public:
	CRoomLobby() {}
	virtual ~CRoomLobby() {}

protected:

public:

};
