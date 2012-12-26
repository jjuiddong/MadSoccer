
#pragma once


#include <Windows.h>
#include <winsock.h>
#include <list>
#include <vector>
#include <string>
#include <algorithm>


typedef int netid;
#define INVALID_NETID		(-1)	// �߸��� NetId�� ���Ѵ�. ���������� ����
#define SERVER_NETID		(0)		// Ŭ���̾�Ʈ �ϰ�� ���� ����� Server�� NetId�� ����


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






// fastmemloader�� ������ �д´�.
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

