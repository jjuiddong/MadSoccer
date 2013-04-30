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
class CFarmServer : public network::CServer
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
	virtual void	OnConnectMultiPlug() override;

	// Network Packet Handling
	virtual bool ReqSubServerLogin(network::IProtocolDispatcher &dispatcher, netid senderId, const std::string &svrType) override;
	virtual bool SendSubServerP2PCLink(network::IProtocolDispatcher &dispatcher, netid senderId, const std::vector<std::string> &v) override;
	virtual bool SendSubServerP2PSLink(network::IProtocolDispatcher &dispatcher, netid senderId, const std::vector<std::string> &v) override;
	virtual bool SendSubServerInputLink(network::IProtocolDispatcher &dispatcher, netid senderId, const std::vector<std::string> &v) override;
	virtual bool SendSubServerOutputLink(network::IProtocolDispatcher &dispatcher, netid senderId, const std::vector<std::string> &v) override;
	virtual bool ReqServerInfoList(network::IProtocolDispatcher &dispatcher, netid senderId, const std::string &clientSvrType, const std::string &serverSvrType) override;
	virtual bool ReqToBindOuterPort(network::IProtocolDispatcher &dispatcher, netid senderId, const std::string &bindSubServerSvrType) override;
	virtual bool ReqToBindInnerPort(network::IProtocolDispatcher &dispatcher, netid senderId, const std::string &bindSubServerSvrType) override;
	virtual bool ReqSubServerBindComplete(network::IProtocolDispatcher &dispatcher, netid senderId, const std::string &bindSubServerSvrType) override;
	virtual bool ReqSubClientConnectComplete(network::IProtocolDispatcher &dispatcher, netid senderId, const std::string &bindSubServerSvrType) override;

private:
	farm::s2c_Protocol m_Protocol;
	ServerBasicPtr m_pServer;

};
