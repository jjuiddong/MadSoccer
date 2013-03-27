//------------------------------------------------------------------------
// Name:    VClient.h
// Author:  jjuiddong
// Date:    1/3/2013
// 
// virtualClient 의 Client클래스
//------------------------------------------------------------------------
#pragma once

#include "NetCommon/Src/basic_ProtocolListener.h"
#include "NetCommon/Src/basic_Protocol.h"

DECLARE_TYPE_NAME(CVClient);
class CVClient : public network::CClient
						,public network::IClientEventListener
						,public all::ProtocolListener
						,public basic::s2c_ProtocolListener
						,public common::CSingleton<CVClient>
						,public memmonitor::Monitor<CVClient, TYPE_NAME(CVClient)>
{
public:
	CVClient();
	virtual ~CVClient();

protected:
	all::Protocol		m_Protocol;

public:
	all::Protocol&		GetProtocol() { return m_Protocol; }

	// Client Event Handler
	virtual void		OnClientConnect(ClientBasicPtr client) override;
	virtual void		OnClientDisconnect(ClientBasicPtr client) override;

	virtual void		OnP2PConnect(ClientBasicPtr client) override;
	virtual void		OnP2PDisconnect(ClientBasicPtr client) override;
	virtual void		OnMemberJoin(ClientBasicPtr client, netid clientId) override;
	virtual void		OnMemberLeave(ClientBasicPtr client, netid clientId) override;

	// network
	// all::protocol
	virtual void recv(netid senderId, network::CPacket &packet) override;

	// basic
	virtual void AckGroupList(netid senderId, const network::error::ERROR_CODE &errorCode, const GroupVector &groups) override;

};
