
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
}
typedef common::ReferencePtr<network::CServer> ServerPtr;
typedef common::ReferencePtr<network::CClient> ClientPtr;
typedef common::ReferencePtr<network::CNetConnector> NetConnectorPtr;




#include "ErrReport/Error.h"
#include "Packet.h"
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


typedef common::ReferencePtr<network::IProtocolListener> ProtocolListenerPtr;
typedef std::list<ProtocolListenerPtr> ProtocolListenerList;
typedef ProtocolListenerList::iterator ProtocolListenerItor;
typedef std::map<int,ProtocolListenerList> ProtocolListenerMap;
typedef ProtocolListenerMap::iterator ProtocolListenerMapItor;

typedef common::ReferencePtr<network::IProtocol> ProtocolPtr;
typedef std::map<int,ProtocolPtr> ProtocolMap;
typedef ProtocolMap::iterator ProtocolItor;

#include "Protocol.h"
#include "ProtocolListener.h"




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

