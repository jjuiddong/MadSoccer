//------------------------------------------------------------------------
// Name:    Server.h
// Author:  jjuiddong
// Date:    2013-03-12
// 
// Group Management
//------------------------------------------------------------------------
#pragma once

#include "ServerBasic.h"
#include "NetCommon/Src/basic_ProtocolListener.h"
#include "NetCommon/Src/basic_Protocol.h"

namespace network
{
	class CServer : public CServerBasic
							, public basic::c2s_ProtocolListener
	{
	public:
		CServer(PROCESS_TYPE procType);
		virtual ~CServer();

	protected:
		// Network Handler
		virtual void ReqGroupList(netid senderId, const netid &groupid) override;
		virtual void ReqGroupJoin(netid senderId, const netid &groupid) override;
		virtual void ReqGroupCreate(netid senderId, const netid &parentGroupId, const std::string &groupName) override;
		virtual void ReqP2PConnect(netid senderId) override;

	protected:
		// protocols
		basic::s2c_Protocol		m_BasicProtocol;

	};
}
