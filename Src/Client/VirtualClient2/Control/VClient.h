/**
Name:    VClient.h
Author:  jjuiddong
Date:    1/3/2013

virtualClient 의 Client클래스
*/
#pragma once

#include "NetProtocol/Src/basic_ProtocolListener.h"
#include "NetProtocol/Src/basic_Protocol.h"
#include "NetProtocol/Src/login_ProtocolListener.h"
#include "NetProtocol/Src/login_Protocol.h"
#include "NetProtocol/Src/p2pComm_ProtocolListener.h"
#include "NetProtocol/Src/p2pComm_Protocol.h"


DECLARE_TYPE_NAME(CVClient);
class CVClient : public network::CClient
						,public all::ProtocolListener
						,public basic::s2c_ProtocolListener
						,public login::s2c_ProtocolListener
						,public p2pComm::c2c_ProtocolListener
						,public common::CSingleton<CVClient>
						,public memmonitor::Monitor<CVClient, TYPE_NAME(CVClient)>
{
public:
	CVClient();
	virtual ~CVClient();

	netid				GetHeroId() const;
	all::Protocol&	GetProtocol() { return m_Protocol; }

	// Client Event Handler
	void		OnConnect(network::CNetEvent &event);
	void		OnDisconnect(network::CNetEvent &event);
	void		OnP2PConnect(network::CNetEvent &event);
	void		OnP2PDisconnect(network::CNetEvent &event);
	void		OnMemberJoin(network::CNetEvent &event);
	void		OnMemberLeave(network::CNetEvent &event);

	// network
	// all::protocol
	virtual void recv(netid senderId, network::CPacket &packet) override;

	// basic
	virtual bool AckGroupList(netid senderId, const network::error::ERROR_CODE &errorCode, const GroupVector &groups) override;
	virtual bool AckLogIn(netid senderId, const network::error::ERROR_CODE &errorCode, const std::string &id, const netid &netId) override;

	// p2pComm
	virtual bool SendData(netid senderId) override;

private:
	netid				m_heroId;
	all::Protocol		m_Protocol;
	p2pComm::c2c_Protocol m_P2pProtocol;

};

netid CVClient::GetHeroId() const { return m_heroId; }
