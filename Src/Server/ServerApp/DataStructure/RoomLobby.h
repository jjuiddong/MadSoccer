//------------------------------------------------------------------------
// Name:    RoomLobby.h
// Author:  jjuiddong
// Date:    12/29/2012
// 
// �κ񼭹��� �����ϴ� �������� �����Ѵ�.
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
