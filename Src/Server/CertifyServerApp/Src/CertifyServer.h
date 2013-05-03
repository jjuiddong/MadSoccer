/**
Name:   CertifyServer.h
Author:  jjuiddong
Date:    4/18/2013

 인증 서버
*/
#pragma once

#include "NetProtocol/src/certify_Protocol.h"
#include "NetProtocol/src/certify_ProtocolListener.h"

class CGlobalRemoteClient;

DECLARE_TYPE_NAME(CCertifyServer)
class CCertifyServer : public network::CServer
								  ,public certify::s2s_ProtocolListener

								  ,public network::AllProtocolDisplayer
								  ,public memmonitor::Monitor<CCertifyServer, TYPE_NAME(CCertifyServer)>
{

	typedef common::StableVectorMap<std::string, CGlobalRemoteClient*> SessionsId;

public:
	CCertifyServer();
	virtual ~CCertifyServer();

protected:
	bool				AddClient( const std::string &id, const std::string &svrType, const certify_key key );
	bool				RemoveClient( const std::string &id );
	CGlobalRemoteClient* GetClient( const std::string &id );
	void				Clear();

	// Event Handler
	virtual void	OnConnectMultiPlug() override;

	// Network Protocol Handler
	virtual bool ReqUserLogin(network::IProtocolDispatcher &dispatcher, netid senderId, const std::string &id, const std::string &passwd, const std::string &svrType) override;
	virtual bool ReqUserLogout(network::IProtocolDispatcher &dispatcher, netid senderId, const std::string &id) override;
	virtual bool ReqUserMoveServer(network::IProtocolDispatcher &dispatcher, netid senderId, const std::string &id, const std::string &svrType) override;

private:
	SessionsId	m_Sessions;
	certify::s2s_Protocol m_Protocol;

};
