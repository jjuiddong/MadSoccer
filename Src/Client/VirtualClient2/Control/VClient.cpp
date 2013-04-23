
#include "stdafx.h"
#include "VClient.h"
#include "../VirtualClient2.h"
#include "../UI/DlgLog.h"
#include "../UI/ProtocolTree.h"
#include "../ui/VirtualClient2Dlg.h"

#include "NetProtocol/Src/basic_ProtocolListener.cpp"
#include "NetProtocol/Src/basic_Protocol.cpp"
#include "NetProtocol/Src/p2pComm_ProtocolListener.cpp"
#include "NetProtocol/Src/p2pComm_Protocol.cpp"
#include "NetProtocol/Src/login_Protocol.cpp"
#include "NetProtocol/Src/login_ProtocolListener.cpp"

using namespace network;

CVClient::CVClient() : CClient(network::USER_LOOP)
{
	RegisterProtocol(&m_Protocol);
	RegisterProtocol(&m_P2pProtocol);
	AddProtocolListener(this);

	EVENT_CONNECT( EVT_CONNECT, CVClient, CVClient::OnConnect );
	EVENT_CONNECT( EVT_DISCONNECT, CVClient, CVClient::OnDisconnect );
	EVENT_CONNECT( EVT_P2P_CONNECT, CVClient, CVClient::OnP2PConnect);
	EVENT_CONNECT( EVT_P2P_DISCONNECT, CVClient, CVClient::OnP2PDisconnect);
	EVENT_CONNECT( EVT_MEMBER_JOIN, CVClient, CVClient::OnMemberJoin);
	EVENT_CONNECT( EVT_MEMBER_LEAVE, CVClient, CVClient::OnMemberLeave);	
}

CVClient::~CVClient()
{

}


//------------------------------------------------------------------------
// 서버로부터 받은 모든 패킷을 추가한다.
//------------------------------------------------------------------------
void CVClient::recv(netid senderId, network::CPacket &packet)
{
	const int protocolID = packet.GetProtocolId();
	const int packetID = packet.GetPacketId();
	sProtocol *protocol = GetTree()->GetProtocol(protocolID, packetID);

	std::stringstream ss;
	ss << "Recv = ";
	ss << network::Packet2String(packet, protocol);

	GetConsole()->AddString( ss.str() );
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool CVClient::AckGroupList(netid senderId, const network::error::ERROR_CODE &errorCode, const GroupVector &groups)
{
	GetConsole()->AddString( "------------" );
	GetConsole()->AddString( 
		common::format( "group count: %d, errorCode: %d",
		groups.size(), errorCode) );		

	for (u_int i=0; i < groups.size(); ++i)
	{
		GetConsole()->AddString( "Group Information" );

		GetConsole()->AddString( 
			common::format( "    group id: %d",
				groups[ i].GetId()) );		

		GetConsole()->AddString( 
			common::format( "    group name: %s",
			groups[ i].GetName().c_str()) );		

		GetConsole()->AddString( 
			common::format( "    users: %d",
			groups[ i].GetUsers().size()) );
	}

	return true;
}



//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CVClient::OnConnect(network::CNetEvent &event)
{
	GetConsole()->AddString( "Connect" );
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CVClient::OnDisconnect(network::CNetEvent &event)
{
	GetConsole()->AddString( "DisConnect" );
}


/**
 @brief 
 */
void	CVClient::OnP2PConnect(network::CNetEvent &event)
{
	GetConsole()->AddString( "P2P Connect" );
}


/**
 @brief 
 */
void	CVClient::OnP2PDisconnect(network::CNetEvent &event)
{
	GetConsole()->AddString( "P2P Disconnect" );
}


/**
 @brief 
 */
void	CVClient::OnMemberJoin(network::CNetEvent &event)
{
	GetConsole()->AddString( "Member Join" );
}


/**
 @brief 
 */
void	CVClient::OnMemberLeave(network::CNetEvent &event)
{
	GetConsole()->AddString( "Member Leave" );
}


/**
 @brief 
 */
bool CVClient::SendData(netid senderId)
{
	// Host 일때, 나머지 클라이언트들에게 패킷을 보낸다.
	if (IsP2PHostClient())
	{
		m_P2pProtocol.SendData( P2P_NETID, SEND_T );
	}
	return true;
}


/**
 @brief Acknowledge
 */
bool CVClient::AckLogIn(netid senderId, const network::error::ERROR_CODE &errorCode, 
	const std::string &id, const netid &netId)
{
	if (errorCode == error::ERR_SUCCESS)
	{
		m_heroId = netId;
		theApp.GetMainDlg()->SetWindowText( 
			common::formatw( "netid = %d", netId).c_str() );			
	}
	else
	{
		theApp.GetMainDlg()->SetWindowText( _T("Login Fail") );
	}
	return true;
}
