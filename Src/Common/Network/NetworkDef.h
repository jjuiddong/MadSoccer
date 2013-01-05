
#pragma once


#include <Windows.h>
#include <winsock.h>
#include <list>
#include <vector>
#include <string>
#include <algorithm>


typedef int netid;
#define INVALID_NETID		(-1)	// 잘못된 NetId를 뜻한다. 오류값으로 쓰임
#define SERVER_NETID		(0)		// 클라이언트 일경우 현재 연결된 Server의 NetId를 뜻함


namespace network
{
	class CServer;
	class CClient;
	class CNetConnector;
	class IProtocol;
	class IProtocolListener;
	class CCharacter;
	class CGroup;
	class CRoom;
	class CUser;
}
typedef common::ReferencePtr<network::CServer> ServerPtr;
typedef common::ReferencePtr<network::CClient> ClientPtr;
typedef common::ReferencePtr<network::CNetConnector> NetConnectorPtr;
typedef common::ReferencePtr<network::CCharacter> CharacterPtr;




#include "ErrReport/Error.h"
#include "DataStructure/Packet.h"
#include "Controller/RemoteClient.h"
#include "Algorithm/BindCore.h"


typedef std::list<SOCKET> SocketList;
typedef SocketList::iterator SockItor;

typedef std::list<netid> NetIdList;
typedef NetIdList::iterator NetIdItor;

typedef std::map<netid,network::CRemoteClient*> RemoteClientMap;
typedef RemoteClientMap::iterator RemoteClientItor;
typedef common::ReferencePtr<network::CRemoteClient> RemoteClientPtr;

typedef std::list<network::CPacket> PacketList;
typedef PacketList::iterator PacketItor;
typedef PacketList::const_iterator PacketCItor;

typedef std::list<network::CGroup*> GroupList;
typedef GroupList::iterator GroupItor;
typedef common::ReferencePtr<network::CGroup> GroupPtr;

typedef std::map<netid,network::CUser*> UserMap;
typedef UserMap::iterator UserItor;
typedef common::ReferencePtr<network::CUser> UserPtr;

typedef std::map<int,network::CRoom*> RoomMap;
typedef RoomMap::iterator RoomItor;
typedef common::ReferencePtr<network::CRoom> RoomPtr;

typedef common::ReferencePtr<network::IProtocolListener> ProtocolListenerPtr;
typedef std::list<ProtocolListenerPtr> ProtocolListenerList;
typedef ProtocolListenerList::iterator ProtocolListenerItor;
typedef std::map<int,ProtocolListenerList> ProtocolListenerMap;
typedef ProtocolListenerMap::iterator ProtocolListenerMapItor;

typedef common::ReferencePtr<network::IProtocol> ProtocolPtr;
typedef std::map<int,ProtocolPtr> ProtocolMap;
typedef ProtocolMap::iterator ProtocolItor;

#include "Interface/Protocol.h"
#include "Interface/ProtocolListener.h"
#include "Interface/ProtocolDispatcher.h"
#include "DataStructure/Group.h"
#include "DataStructure/Room.h"
#include "DataStructure/User.h"
#include "Service/AllProtocol.h"
#include "Service/AllProtocolListener.h"



// fastmemloader로 정보를 읽는다.
#pragma pack(1)
typedef struct _SServerInfo
{
	char	name[32];
	char	s2sip[32];
	int		s2sport;
	int		s2cport;
} SServerInfo;

typedef struct _SServerLaunchInfo
{
	int lobbyServerCnt;
	SServerInfo *lobbySvrs;
	int gameServerCnt;
	SServerInfo *gameSvrs;

} SServerLaunchInfo;
#pragma pack()

