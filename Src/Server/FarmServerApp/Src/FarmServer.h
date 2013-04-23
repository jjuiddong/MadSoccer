/**
Name:   FarmServer.h
현Author:  jjuiddong
Date:    4/7/2013

 팜 서버 구현 
*/
#pragma once

#include "NetProtocol\Src\farm_Protocol.h"
#include "NetProtocol\Src\farm_ProtocolListener.h"

DECLARE_TYPE_NAME(CFarmServer)
class CFarmServer : public network::multinetwork::CNetGroupDelegation
								,public farm::c2s_ProtocolListener
								,public network::AllProtocolDisplayer
								,public memmonitor::Monitor<CFarmServer, TYPE_NAME(CFarmServer)>
{
public:
	CFarmServer();
	virtual ~CFarmServer();

	SubServerGroupPtr FindGroup(const std::string &svrType);

private:
	// Event Handling
	virtual void	OnConnectNetGroupController() override;

	// Network Packet Handling
	virtual bool ReqSubServerLogin(netid senderId, const std::string &svrType) override;
	virtual bool SendSubServerP2PCLink(netid senderId, const std::vector<std::string> &v) override;
	virtual bool SendSubServerP2PSLink(netid senderId, const std::vector<std::string> &v) override;
	virtual bool SendSubServerInputLink(netid senderId, const std::vector<std::string> &v) override;
	virtual bool SendSubServerOutputLink(netid senderId, const std::vector<std::string> &v) override;
	virtual bool ReqServerInfoList(netid senderId, const std::string &clientSvrType, const std::string &serverSvrType) override;
	virtual bool ReqToBindOuterPort(netid senderId, const std::string &bindSubServerSvrType) override;
	virtual bool ReqToBindInnerPort(netid senderId, const std::string &bindSubServerSvrType) override;
	virtual bool ReqSubServerBindComplete(netid senderId, const std::string &bindSubServerSvrType) override;
	virtual bool ReqSubClientConnectComplete(netid senderId, const std::string &bindSubServerSvrType) override;

private:
	farm::s2c_Protocol m_Protocol;
	ServerBasicPtr m_pServer;

};
