/**
Name:   CertifyServer.h
Author:  jjuiddong
Date:    4/18/2013

 인증 서버
*/
#pragma once

DECLARE_TYPE_NAME(CCertifyServer)
class CCertifyServer : public network::multinetwork::CNetGroupDelegation
								  ,public network::AllProtocolDisplayer
								  ,public memmonitor::Monitor<CCertifyServer, TYPE_NAME(CCertifyServer)>
{
public:
	CCertifyServer();
	virtual ~CCertifyServer();


};
