/**
Name:   Client.h
Author:  jjuiddong
Date:    3/17/2013

*/
#pragma once

#include "ClientBasic.h"

namespace network
{
	class CBasicS2CProtocolHandler;

	class CClient : public CClientBasic
	{
	public:
		CClient(PROCESS_TYPE procType);
		virtual ~CClient();

	protected:
		CBasicS2CProtocolHandler *m_pBasicS2CProtocolHandler;
	};

};
