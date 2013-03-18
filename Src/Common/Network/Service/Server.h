//------------------------------------------------------------------------
// Name:    Server.h
// Author:  jjuiddong
// Date:    2013-03-12
// 
// Group Management
//------------------------------------------------------------------------
#pragma once

#include "ServerBasic.h"

namespace basic { class s2c_Protocol; }

namespace network
{
	class CBasicC2SProtocolHandler;
	class CServer : public CServerBasic
	{
		friend class CBasicC2SProtocolHandler;

	public:
		CServer(PROCESS_TYPE procType);
		virtual ~CServer();

	protected:
		// Network Handler

	protected:
		// protocols
		basic::s2c_Protocol		*m_pBasicProtocol;
		CBasicC2SProtocolHandler *m_pBasicPrtHandler;
	};
}
