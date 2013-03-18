
#include "stdafx.h"
#include "Client.h"
#include "../ProtocolHandler/BasicS2CProtocolHandler.h"

using namespace network;


CClient::CClient(PROCESS_TYPE procType) : 
	CClientBasic(procType)
,	m_pBasicS2CProtocolHandler(NULL)
{

	m_pBasicS2CProtocolHandler = new CBasicS2CProtocolHandler(*this);
	AddProtocolListener(m_pBasicS2CProtocolHandler);
}

CClient::~CClient()
{
	RemoveProtocolListener(m_pBasicS2CProtocolHandler);
	SAFE_DELETE(m_pBasicS2CProtocolHandler);

}

