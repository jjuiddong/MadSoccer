
#pragma once


#include <Windows.h>
#include <winsock.h>
#include <list>
#include <vector>
#include <string>
#include <algorithm>

#include "Packet.h"
#include "PacketListener.h"

typedef std::list<SOCKET> SocketList;
typedef SocketList::iterator SockItor;

typedef std::list<network::CPacket> PacketList;
typedef PacketList::iterator PacketItor;
typedef PacketList::const_iterator PacketCItor;

typedef std::list<network::IPacketListener*> ListenerList;
typedef ListenerList::iterator ListenerItor;


#include "PacketDispatcher.h"



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

