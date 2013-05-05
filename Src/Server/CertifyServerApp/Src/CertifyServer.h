/**
Name:   CertifyServer.h
Author:  jjuiddong
Date:    4/18/2013

 인증 서버
*/
#pragma once

#include "NetProtocol/src/certify_Protocol.h"
#include "NetProtocol/src/certify_ProtocolListener.h"
#include "NetProtocol/src/server_network_Protocol.h"
#include "NetProtocol/Src/server_network_ProtocolListener.h"

class CGlobalRemotePlayer;

DECLARE_TYPE_NAME(CCertifyServer)
class CCertifyServer : public network::CServer
								  ,public certify::s2s_ProtocolListener
								  ,public server_network::s2s_ProtocolListener

								  ,public network::AllProtocolDisplayer
								  ,public memmonitor::Monitor<CCertifyServer, TYPE_NAME(CCertifyServer)>
{

	typedef common::StableVectorMap<std::string, CGlobalRemotePlayer*> SessionsId;

public:
	CCertifyServer();
	virtual ~CCertifyServer();

protected:
	bool				AddPlayer( const std::string &id, const std::string &svrType, const netid svrId, const certify_key key );
	bool				RemovePlayer( const std::string &id );
	CGlobalRemotePlayer* GetPlayer( const std::string &id );
	void				PlayerLogoutConnectingServer( const std::string &svrType );
	void				Clear();

	// Event Handler
	virtual void	OnConnectMultiPlug() override;
	virtual void	OnClientLeave(network::CNetEvent &event);

	// Network Protocol Handler
	virtual bool ReqUserLogin(network::IProtocolDispatcher &dispatcher, netid senderId, const std::string &id, const std::string &passwd, const std::string &svrType) override;
	virtual bool ReqUserLogout(network::IProtocolDispatcher &dispatcher, netid senderId, const std::string &id) override;
	virtual bool ReqUserMoveServer(network::IProtocolDispatcher &dispatcher, netid senderId, const std::string &id, const std::string &svrType) override;

	virtual bool SendServerInfo(network::IProtocolDispatcher &dispatcher, netid senderId, const std::string &svrType, const std::string &ip, const int &port, const int &userCount) override;

private:
	SessionsId	m_Sessions;
	certify::s2s_Protocol m_Protocol;

};
