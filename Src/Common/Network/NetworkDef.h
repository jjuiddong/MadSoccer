
#pragma once


#include <Windows.h>
#include <winsock.h>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include "Common/Common.h"
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
	class CRoom;
	class CUser;

}

typedef common::ReferencePtr<network::CServerBasic> ServerBasicPtr;
typedef common::ReferencePtr<network::CClientBasic> ClientBasicPtr;
typedef common::ReferencePtr<network::CClient> ClientPtr;
typedef common::ReferencePtr<network::CP2PClient> P2PClientPtr;
typedef common::ReferencePtr<network::CCoreClient> CoreClientPtr;
typedef common::ReferencePtr<network::CCharacter> CharacterPtr;

typedef common::VectorMap<netid, ClientBasicPtr> Clients;
typedef common::VectorMap<netid, network::CClientBasic*> Clients_;
typedef Clients_::VectorType	Clients_V;
typedef common::VectorMap<netid, CoreClientPtr> CoreClients;
typedef common::VectorMap<netid, network::CCoreClient*> CoreClients_;
typedef CoreClients_::VectorType	CoreClients_V;

//typedef common::VectorMap<netid, CCoreClient*> CoreClientsPtr;



#include "ErrReport/ErrorCode.h"
#include "ErrReport/ErrorCheck.h"
#include "Algorithm/BindCore.h"


typedef std::map<netid,network::CUser*> UserMap;
typedef UserMap::iterator UserItor;
typedef common::ReferencePtr<network::CUser> UserPtr;

typedef std::map<int,network::CRoom*> RoomMap;
typedef RoomMap::iterator RoomItor;
typedef common::ReferencePtr<network::CRoom> RoomPtr;


#include "Utility/Utility.h"

#include "DataStructure/Room.h"
#include "DataStructure/User.h"
#include "Service/AllProtocol.h"
#include "Service/AllProtocolListener.h"


typedef std::vector<network::CGroup> GroupVector;



namespace network { namespace multinetwork {

	class CNetGroupController;
	class CNetGroupDelegation;
}}

typedef common::ReferencePtr<network::multinetwork::CNetGroupController> NetGroupControllerPtr;
typedef common::ReferencePtr<network::multinetwork::CNetGroupDelegation> NetGroupDelegationPtr;


#include "Controller/CoreClient.h"

#include "MultiNetwork/MultiNetwork.h"
#include "MultiNetwork/NetGroupController.h"
#include "MultiNetwork/NetGroupDelegation.h"

#include "ProtocolHandler\AllProtocolDisplayer.h"



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

