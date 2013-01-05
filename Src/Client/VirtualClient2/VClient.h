//------------------------------------------------------------------------
// Name:    VClient.h
// Author:  jjuiddong
// Date:    1/3/2013
// 
// virtualClient 의 Client클래스
//------------------------------------------------------------------------
#pragma once


class CVClient : public network::CClient, public common::CSingleton<CVClient>
				, public all::ProtocolListener
{
public:
	CVClient();
	virtual ~CVClient() {}

protected:
	all::Protocol		m_Protocol;

public:
	all::Protocol&		GetProtocol() { return m_Protocol; }

	virtual void OnConnect() override;
	virtual void OnDisconnect() override;

	// network
	// all::protocol
	virtual void recv(netid senderId, network::CPacket &packet) override;

};
