
#include "stdafx.h"
#include "server.h"
#include "NetProtocol/Src/basic_Protocol.h"
#include "../ProtocolHandler/BasicC2SProtocolHandler.h"


using namespace network;

CServer::CServer(PROCESS_TYPE procType) :
	CServerBasic(procType)
,	m_pBasicProtocol(NULL)
,	m_pBasicPrtHandler(NULL)
{
	m_pBasicProtocol = new basic::s2c_Protocol();
	m_pBasicProtocol->SetNetConnector(this);

	m_pBasicPrtHandler = new CBasicC2SProtocolHandler(*this);
	AddProtocolListener(m_pBasicPrtHandler);

}

CServer::~CServer()
{
	RemoveProtocolListener(m_pBasicPrtHandler);
	SAFE_DELETE(m_pBasicPrtHandler);

	SAFE_DELETE(m_pBasicProtocol);
}

