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
	virtual void ReqSubServerLogin(netid senderId, const std::string &svrType) override;
	virtual void SendSubServerP2PCLink(netid senderId, const std::vector<std::string> &v) override;
	virtual void SendSubServerP2PSLink(netid senderId, const std::vector<std::string> &v) override;
	virtual void SendSubServerInputLink(netid senderId, const std::vector<std::string> &v) override;
	virtual void SendSubServerOutputLink(netid senderId, const std::vector<std::string> &v) override;
	virtual void ReqServerInfoList(netid senderId, const std::string &clientSvrType, const std::string &serverSvrType) override;
	virtual void ReqToBindOuterPort(netid senderId, const std::string &bindSubServerSvrType) override;
	virtual void ReqToBindInnerPort(netid senderId, const std::string &bindSubServerSvrType) override;
	virtual void ReqSubServerBindComplete(netid senderId, const std::string &bindSubServerSvrType) override;
	virtual void ReqSubClientConnectComplete(netid senderId, const std::string &bindSubServerSvrType) override;

private:
	farm::s2c_Protocol m_Protocol;
	ServerBasicPtr m_pServer;

};
