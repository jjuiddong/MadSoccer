#include "basic_ProtocolListener.h"
#include "Network/Controller/NetController.h"
#include "NetCommon/PrtCompiler/ProtocolMacro.h"

using namespace network;
using namespace marshalling;
using namespace basic;

static basic::s2c_Dispatcher g_basic_s2c_Dispatcher;

basic::s2c_Dispatcher::s2c_Dispatcher()
	: IProtocolDispatcher(basic::s2c_Dispatcher_ID)
{
	CNetController::Get()->AddDispatcher(this);
}

//------------------------------------------------------------------------
// 패킷의 프로토콜에 따라 해당하는 리스너의 함수를 호출한다.
//------------------------------------------------------------------------
void basic::s2c_Dispatcher::Dispatch(CPacket &packet, const ProtocolListenerList &listeners)
{
	const int protocolId = packet.GetProtocolId();
	const int packetId = packet.GetPacketId();
	switch (packetId)
	{
	case 501:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<s2c_ProtocolListener>(listeners, recvListener))
				break;

			network::error::ERROR_CODE errorCode;
			packet >> errorCode;
			GroupVector groups;
			packet >> groups;
			SEND_LISTENER(s2c_ProtocolListener, recvListener, AckGroupList(packet.GetSenderId(), errorCode, groups) );
		}
		break;

	case 502:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<s2c_ProtocolListener>(listeners, recvListener))
				break;

			network::error::ERROR_CODE errorCode;
			packet >> errorCode;
			netid reqId;
			packet >> reqId;
			netid joinGroupId;
			packet >> joinGroupId;
			SEND_LISTENER(s2c_ProtocolListener, recvListener, AckGroupJoin(packet.GetSenderId(), errorCode, reqId, joinGroupId) );
		}
		break;

	case 503:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<s2c_ProtocolListener>(listeners, recvListener))
				break;

			network::error::ERROR_CODE errorCode;
			packet >> errorCode;
			netid reqId;
			packet >> reqId;
			netid crGroupId;
			packet >> crGroupId;
			netid crParentGroupId;
			packet >> crParentGroupId;
			std::string groupName;
			packet >> groupName;
			SEND_LISTENER(s2c_ProtocolListener, recvListener, AckGroupCreate(packet.GetSenderId(), errorCode, reqId, crGroupId, crParentGroupId, groupName) );
		}
		break;

	case 504:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<s2c_ProtocolListener>(listeners, recvListener))
				break;

			network::error::ERROR_CODE errorCode;
			packet >> errorCode;
			netid reqId;
			packet >> reqId;
			netid crGroupId;
			packet >> crGroupId;
			netid crParentGroupId;
			packet >> crParentGroupId;
			std::string groupName;
			packet >> groupName;
			SEND_LISTENER(s2c_ProtocolListener, recvListener, AckGroupCreateBlank(packet.GetSenderId(), errorCode, reqId, crGroupId, crParentGroupId, groupName) );
		}
		break;

	case 505:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<s2c_ProtocolListener>(listeners, recvListener))
				break;

			netid toGroupId;
			packet >> toGroupId;
			netid fromGroupId;
			packet >> fromGroupId;
			netid userId;
			packet >> userId;
			SEND_LISTENER(s2c_ProtocolListener, recvListener, JoinMember(packet.GetSenderId(), toGroupId, fromGroupId, userId) );
		}
		break;

	case 506:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<s2c_ProtocolListener>(listeners, recvListener))
				break;

			network::error::ERROR_CODE errorCode;
			packet >> errorCode;
			network::P2P_STATE state;
			packet >> state;
			std::string ip;
			packet >> ip;
			int port;
			packet >> port;
			SEND_LISTENER(s2c_ProtocolListener, recvListener, AckP2PConnect(packet.GetSenderId(), errorCode, state, ip, port) );
		}
		break;

	case 507:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<s2c_ProtocolListener>(listeners, recvListener))
				break;

			SEND_LISTENER(s2c_ProtocolListener, recvListener, func1(packet.GetSenderId()) );
		}
		break;

	case 508:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<s2c_ProtocolListener>(listeners, recvListener))
				break;

			std::string str;
			packet >> str;
			SEND_LISTENER(s2c_ProtocolListener, recvListener, func2(packet.GetSenderId(), str) );
		}
		break;

	case 509:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<s2c_ProtocolListener>(listeners, recvListener))
				break;

			float value;
			packet >> value;
			SEND_LISTENER(s2c_ProtocolListener, recvListener, func3(packet.GetSenderId(), value) );
		}
		break;

	case 510:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<s2c_ProtocolListener>(listeners, recvListener))
				break;

			SEND_LISTENER(s2c_ProtocolListener, recvListener, func4(packet.GetSenderId()) );
		}
		break;

	case 511:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<s2c_ProtocolListener>(listeners, recvListener))
				break;

			std::string ok;
			packet >> ok;
			float a;
			packet >> a;
			int b;
			packet >> b;
			SEND_LISTENER(s2c_ProtocolListener, recvListener, func5(packet.GetSenderId(), ok, a, b) );
		}
		break;

	default:
		assert(0);
		break;
	}
}



static basic::c2s_Dispatcher g_basic_c2s_Dispatcher;

basic::c2s_Dispatcher::c2s_Dispatcher()
	: IProtocolDispatcher(basic::c2s_Dispatcher_ID)
{
	CNetController::Get()->AddDispatcher(this);
}

//------------------------------------------------------------------------
// 패킷의 프로토콜에 따라 해당하는 리스너의 함수를 호출한다.
//------------------------------------------------------------------------
void basic::c2s_Dispatcher::Dispatch(CPacket &packet, const ProtocolListenerList &listeners)
{
	const int protocolId = packet.GetProtocolId();
	const int packetId = packet.GetPacketId();
	switch (packetId)
	{
	case 601:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<c2s_ProtocolListener>(listeners, recvListener))
				break;

			netid groupid;
			packet >> groupid;
			SEND_LISTENER(c2s_ProtocolListener, recvListener, ReqGroupList(packet.GetSenderId(), groupid) );
		}
		break;

	case 602:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<c2s_ProtocolListener>(listeners, recvListener))
				break;

			netid groupid;
			packet >> groupid;
			SEND_LISTENER(c2s_ProtocolListener, recvListener, ReqGroupJoin(packet.GetSenderId(), groupid) );
		}
		break;

	case 603:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<c2s_ProtocolListener>(listeners, recvListener))
				break;

			netid parentGroupId;
			packet >> parentGroupId;
			std::string groupName;
			packet >> groupName;
			SEND_LISTENER(c2s_ProtocolListener, recvListener, ReqGroupCreate(packet.GetSenderId(), parentGroupId, groupName) );
		}
		break;

	case 604:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<c2s_ProtocolListener>(listeners, recvListener))
				break;

			netid parentGroupId;
			packet >> parentGroupId;
			std::string groupName;
			packet >> groupName;
			SEND_LISTENER(c2s_ProtocolListener, recvListener, ReqGroupCreateBlank(packet.GetSenderId(), parentGroupId, groupName) );
		}
		break;

	case 605:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<c2s_ProtocolListener>(listeners, recvListener))
				break;

			SEND_LISTENER(c2s_ProtocolListener, recvListener, ReqP2PConnect(packet.GetSenderId()) );
		}
		break;

	case 606:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<c2s_ProtocolListener>(listeners, recvListener))
				break;

			bool isSuccess;
			packet >> isSuccess;
			SEND_LISTENER(c2s_ProtocolListener, recvListener, ReqP2PConnectTryResult(packet.GetSenderId(), isSuccess) );
		}
		break;

	case 607:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<c2s_ProtocolListener>(listeners, recvListener))
				break;

			std::string str;
			packet >> str;
			SEND_LISTENER(c2s_ProtocolListener, recvListener, func2(packet.GetSenderId(), str) );
		}
		break;

	case 608:
		{
			ProtocolListenerList recvListener;
			if (!ListenerMatching<c2s_ProtocolListener>(listeners, recvListener))
				break;

			float value;
			packet >> value;
			SEND_LISTENER(c2s_ProtocolListener, recvListener, func3(packet.GetSenderId(), value) );
		}
		break;

	default:
		assert(0);
		break;
	}
}



