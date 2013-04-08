/**
Name:    NetCommon.h
Author:  jjuiddong
Date:    2013-03-26

*/
#pragma once

#include <Windows.h>
#include <winsock.h>


typedef int netid;
#define INVALID_NETID		(-1)		// �߸��� NetId�� ���Ѵ�. ���������� ����
#define SERVER_NETID		(0)		// ����� Server�� NetId�� ���� (Ŭ���̾�Ʈ �� ��)
#define P2P_NETID				(1)		// ����� P2P ��Ʈ��ũ Host NetId�� ���� (Ŭ���̾�Ʈ �� ��)
#define ROOT_GROUP_NETID (-1)	// ������ �ֻ��� �׷��� ���Ѵ�.



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
		USER_LOOP,									// ������ �����忡�� ó��
		SERVICE_SEPERATE_THREAD,		// ���� �����忡�� ó�� (����ó���� ���� ���� �и��ص� �����带 ���Ѵ�)
		SERVICE_EXCLUSIVE_THREAD,		// ������ �����带 �����ؼ� ó��
		SERVICE_CHILD_THREAD,				// �θ� �����忡�� ó���Ѵ�. ���� �θ� USER_LOOP ���, �ڽĵ� USER_LOOP�� �����Ѵ�.
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


