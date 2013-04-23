/**
Name:   BasicC2SProtocolHandler.h
Author:  jjuiddong
Date:    3/17/2013

basic_protocol.prt Protocol Handler
*/
#pragma once

#include "NetProtocol/Src/basic_ProtocolListener.h"
#include "NetProtocol/Src/basic_Protocol.h"

namespace network
{
	class CServer;
	class CBasicC2SProtocolHandler : public basic::c2s_ProtocolListener
	{
	public:
		CBasicC2SProtocolHandler( CServerBasic &svr );
		virtual ~CBasicC2SProtocolHandler();

		CServerBasic& GetServer();
		basic::s2c_Protocol& GetBasicProtocol();

		// Network Protocol Handler
		virtual bool ReqLogIn(netid senderId, const std::string &id, const std::string &passwd) override;
		virtual bool ReqLogOut(netid senderId, const std::string &id) override;
		virtual bool ReqMoveToServer(netid senderId, const std::string &serverName) override;

		virtual bool ReqGroupList(netid senderId, const netid &groupid) override;
		virtual bool ReqGroupJoin(netid senderId, const netid &groupid) override;
		virtual bool ReqGroupCreate(netid senderId, const netid &parentGroupId, const std::string &groupName) override;
		virtual bool ReqGroupCreateBlank(netid senderId, const netid &parentGroupId, const std::string &groupName) override;
		virtual bool ReqP2PConnect(netid senderId) override;

	protected:
		bool			CreateBlankGroup( netid senderId, const netid &parentGroupId, const std::string &groupName, 
			OUT GroupPtr &pParent, OUT GroupPtr &pFrom, OUT GroupPtr &pNew );

	private:
		CServerBasic				&m_Server;		/// CServer Reference 
		basic::s2c_Protocol		m_BasicProtocol;
	};


	inline CServerBasic& CBasicC2SProtocolHandler::GetServer() { return m_Server; }
	inline basic::s2c_Protocol& CBasicC2SProtocolHandler::GetBasicProtocol() { return m_BasicProtocol; }

}
