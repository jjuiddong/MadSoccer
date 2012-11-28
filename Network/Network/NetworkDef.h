
#pragma once


#include <Windows.h>
#include <winsock.h>
#include <list>
#include <string>
#include <algorithm>

#include "Packet.h"


typedef std::list<SOCKET> SocketList;
typedef SocketList::iterator SockItor;

typedef std::list<network::CPacket> PacketList;
typedef PacketList::iterator PacketItor;
typedef PacketList::const_iterator PacketCItor;


