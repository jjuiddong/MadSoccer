//------------------------------------------------------------------------
// Name:    VClient.h
// Author:  jjuiddong
// Date:    1/3/2013
// 
// virtualClient 의 Client클래스
//------------------------------------------------------------------------
#pragma once


class CVClient : public network::CClient
						,public network::IClientEventListener
						,public all::ProtocolListener
						,public basic::s2c_ProtocolListener
						,public common::CSingleton<CVClient>
{
public:
	CVClient();
	virtual ~CVClient();

protected:
	all::Protocol		m_Protocol;

public:
	all::Protocol&		GetProtocol() { return m_Protocol; }

	//virtual void OnConnect() override;
	//virtual void OnDisconnect() override;
	virtual void			OnClientConnect(ClientPtr client) override;
	virtual void			OnClientDisconnect(ClientPtr client) override;


	// network
	// all::protocol
	virtual void recv(netid senderId, network::CPacket &packet) override;

	// basic
	virtual void AckGroupList(netid senderId, const int &errorCode, const GroupVector &groups) override;

};
