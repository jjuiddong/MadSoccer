/**
Name:   BasicC2SProtocolHandler.h
Author:  jjuiddong
Date:    3/17/2013

basic_protocol.prt Protocol Handler
*/

#include "NetCommon/Src/basic_ProtocolListener.h"
#include "NetCommon/Src/basic_Protocol.h"

namespace network
{
	class CServer;
	class CBasicC2SProtocolHandler : public basic::c2s_ProtocolListener
	{
	public:
		CBasicC2SProtocolHandler( CServer &svr );
		virtual ~CBasicC2SProtocolHandler();

		// Network Protocol Handler
		virtual void ReqGroupList(netid senderId, const netid &groupid) override;
		virtual void ReqGroupJoin(netid senderId, const netid &groupid) override;
		virtual void ReqGroupCreate(netid senderId, const netid &parentGroupId, const std::string &groupName) override;
		virtual void ReqGroupCreateBlank(netid senderId, const netid &parentGroupId, const std::string &groupName) override;
		virtual void ReqP2PConnect(netid senderId) override;

	protected:
		bool			CreateBlankGroup( netid senderId, const netid &parentGroupId, const std::string &groupName, 
			OUT GroupPtr &pParent, OUT GroupPtr &pFrom, OUT GroupPtr &pNew );

	protected:
		CServer						&m_Server;		/// CServer Reference 
		basic::s2c_Protocol		m_BasicProtocol;
	};
}
