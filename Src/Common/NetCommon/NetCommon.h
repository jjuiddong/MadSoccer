/**
Name:    NetCommon.h
Author:  jjuiddong
Date:    2013-03-26

*/
#pragma once

#include <Windows.h>
#include <winsock.h>


typedef int netid;
#define INVALID_NETID		(-1)		// 잘못된 NetId를 뜻한다. 오류값으로 쓰임
#define SERVER_NETID		(0)		// 연결된 Server의 NetId를 뜻함 (클라이언트 일 때)
#define P2P_NETID				(1)		// 연결된 P2P 네트워크 Host NetId를 뜻함 (클라이언트 일 때)
#define ROOT_GROUP_NETID (-1)	// 서버의 최상위 그룹을 뜻한다.



namespace network
{
	using namespace common;

	class CNetConnector;
	class CGroup;
	class CRemoteClient;
	class CPacket;
	class IProtocol;
	class IProtocolListener;


	enum SERVICE_TYPE
	{
		CLIENT,
		SERVER,
	};

	enum PROCESS_TYPE
	{
		USER_LOOP,									// 유저의 쓰레드에서 처리
		SERVICE_SEPERATE_THREAD,		// 통합 쓰레드에서 처리 (통합처리를 위해 따로 분리해둔 쓰레드를 뜻한다)
		SERVICE_EXCLUSIVE_THREAD,		// 별도의 쓰레드를 생성해서 처리
		SERVICE_CHILD_THREAD,				// 부모 쓰레드에서 처리한다. 만약 부모가 USER_LOOP 라면, 자식도 USER_LOOP로 동작한다.
	};

	enum P2P_STATE
	{
		P2P_HOST,
		P2P_CLIENT,
	};

	/// Send Packet Flag, composite flag
	enum SEND_FLAG
	{
		SEND_TARGET,
		SEND_VIEWER,
		SEND_TARGET_VIEWER,
		SEND_T = SEND_TARGET,
		SEND_V = SEND_VIEWER,
		SEND_T_V = SEND_TARGET_VIEWER,
	};



	struct SHostInfo
	{
		std::string ip;
		int portnum;
	};

}


typedef std::list<SOCKET> SocketList;
typedef SocketList::iterator SockItor;

typedef std::list<netid> NetIdList;
typedef NetIdList::iterator NetIdItor;
typedef std::vector<netid> NetIdes;

typedef std::list<network::CPacket> PacketList;
typedef PacketList::iterator PacketItor;
typedef PacketList::const_iterator PacketCItor;

typedef common::ReferencePtr<network::CNetConnector> NetConnectorPtr;

typedef std::map<netid,network::CRemoteClient*> RemoteClientMap;
typedef RemoteClientMap::iterator RemoteClientItor;
typedef common::ReferencePtr<network::CRemoteClient> RemoteClientPtr;

typedef std::list<network::CGroup*> GroupList;
typedef GroupList::iterator GroupItor;
typedef common::ReferencePtr<network::CGroup> GroupPtr;
typedef common::VectorMap<netid, network::CGroup*> Groups;

typedef common::ReferencePtr<network::IProtocol> ProtocolPtr;
typedef common::ReferencePtr<network::IProtocolListener> ProtocolListenerPtr;
typedef std::list<ProtocolListenerPtr> ProtocolListenerList;
typedef ProtocolListenerList::iterator ProtocolListenerItor;
typedef std::map<int,ProtocolListenerList> ProtocolListenerMap;
typedef ProtocolListenerMap::iterator ProtocolListenerMapItor;
typedef std::map<int,ProtocolPtr> ProtocolMap;
typedef ProtocolMap::iterator ProtocolItor;



#include "DataStructure/Packet.h"
#include "DataStructure/PacketQueue.h"
#include "DataStructure/RemoteClient.h"
#include "DataStructure/Group.h"
#include "Marshalling/Marshalling.h"
#include "Marshalling/MarshallingGroup.h"

#include "Controller/NetConnector.h"
#include "Controller/NetConnectorLinker.h"

#include "Interface/Protocol.h"
#include "Interface/ProtocolDispatcher.h"
#include "Interface/ProtocolListener.h"
#include "Interface/Factory.h"

#include "PrtCompiler/ProtocolDefine.h"
#include "PrtCompiler/ProtocolParser.h"
#include "PrtCompiler/ProtocolMacro.h"

#include "Event/NetEvent.h"


