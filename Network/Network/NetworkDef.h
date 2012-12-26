
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
	class CNetObject;
}
typedef common::ReferencePtr<network::CServer> ServerPtr;
typedef common::ReferencePtr<network::CClient> ClientPtr;
typedef common::ReferencePtr<network::CNetObject> NetObjectPtr;




#include "ErrReport/Error.h"
#include "Packet.h"
#include "PacketListener.h"
#include "RemoteClient.h"
#include "Algorithm/BindCore.h"



typedef std::list<SOCKET> SocketList;
typedef SocketList::iterator SockItor;

typedef std::list<network::CRemoteClient*> RemoteClientList;
typedef RemoteClientList::iterator RemoteClientItor;
typedef common::ReferencePtr<network::CRemoteClient> RemoteClientPtr;


typedef std::list<network::CPacket> PacketList;
typedef PacketList::iterator PacketItor;
typedef PacketList::const_iterator PacketCItor;

typedef std::list<network::IProtocolListener*> ListenerList;
typedef ListenerList::iterator ListenerItor;



#include "Protocol.h"
typedef common::ReferencePtr<network::IProtocol> ProtocolPtr;






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

