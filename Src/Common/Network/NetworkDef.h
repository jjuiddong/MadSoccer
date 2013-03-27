
#pragma once


#include <Windows.h>
#include <winsock.h>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include "NetCommon/NetCommon.h"

#include "wxMemMonitorLib/wxMemMonitor.h"


namespace network
{
	class CServerBasic;
	class CClientBasic;
	class CClient;
	class CCoreClient;
	class CP2PClient;
	class CCharacter;
	class CGroup;
	class CRoom;
	class CUser;

}

typedef common::ReferencePtr<network::CServerBasic> ServerBasicPtr;
typedef common::ReferencePtr<network::CClientBasic> ClientBasicPtr;
typedef common::ReferencePtr<network::CClient> ClientPtr;
typedef common::ReferencePtr<network::CP2PClient> P2PClientPtr;
typedef common::ReferencePtr<network::CCoreClient> CoreClientPtr;
typedef common::ReferencePtr<network::CCharacter> CharacterPtr;




#include "ErrReport/ErrorCode.h"
#include "Controller/RemoteClient.h"
#include "Algorithm/BindCore.h"

typedef std::list<SOCKET> SocketList;
typedef SocketList::iterator SockItor;

typedef std::list<netid> NetIdList;
typedef NetIdList::iterator NetIdItor;
typedef std::vector<netid> NetIdes;

typedef std::map<netid,network::CRemoteClient*> RemoteClientMap;
typedef RemoteClientMap::iterator RemoteClientItor;
typedef common::ReferencePtr<network::CRemoteClient> RemoteClientPtr;

typedef std::list<network::CPacket> PacketList;
typedef PacketList::iterator PacketItor;
typedef PacketList::const_iterator PacketCItor;

typedef std::list<network::CGroup*> GroupList;
typedef GroupList::iterator GroupItor;
typedef common::ReferencePtr<network::CGroup> GroupPtr;
typedef common::VectorMap<netid, network::CGroup*> Groups;
//typedef Groups::VectorType GroupVector;


typedef std::map<netid,network::CUser*> UserMap;
typedef UserMap::iterator UserItor;
typedef common::ReferencePtr<network::CUser> UserPtr;

typedef std::map<int,network::CRoom*> RoomMap;
typedef RoomMap::iterator RoomItor;
typedef common::ReferencePtr<network::CRoom> RoomPtr;


#include "Utillity/UserAccess.h"
#include "Utillity/Utillity.h"

#include "DataStructure/Group.h"
#include "DataStructure/Room.h"
#include "DataStructure/User.h"
#include "Service/AllProtocol.h"
#include "Service/AllProtocolListener.h"
#include "Marshalling/MarshallingGroup.h"


typedef std::vector<network::CGroup> GroupVector;





// fastmemloader로 정보를 읽는다.
//#pragma pack(1)
//typedef struct _SServerInfo
//{
//	char	name[32];
//	char	s2sip[32];
//	int		s2sport;
//	int		s2cport;
//} SServerInfo;
//
//typedef struct _SServerLaunchInfo
//{
//	int lobbyServerCnt;
//	SServerInfo *lobbySvrs;
//	int gameServerCnt;
//	SServerInfo *gameSvrs;
//
//} SServerLaunchInfo;
//#pragma pack()

