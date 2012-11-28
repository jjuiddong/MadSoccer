
#pragma once


#include <Windows.h>
#include <winsock.h>
#include <list>
#include <string>
#include <algorithm>

#include "Packet.h"


// class CServer;
// class CClient;

enum NETWORK_TYPE
{
	SERVER,
	CLIENT,		
};

typedef struct _SSeverData
{
	int port;

} SSeverData;

typedef struct _SClientData
{
	std::string ip;
	int port;

} SClientData;


typedef std::list<SOCKET> SocketList;
typedef SocketList::iterator SockItor;

typedef std::list<network::CPacket> PacketList;
typedef PacketList::iterator PacketItor;
typedef PacketList::const_iterator PacketCItor;


