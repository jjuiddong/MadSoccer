
#include "stdafx.h"
#include "server.h"
#include "NetProtocol/Src/basic_Protocol.h"
#include "../ProtocolHandler/BasicC2SProtocolHandler.h"


using namespace network;

CServer::CServer(PROCESS_TYPE procType) :
	//CServerBasic(procType)
	m_pBasicProtocol(NULL)
,	m_pBasicPrtHandler(NULL)
{

}

CServer::~CServer()
{
	RemoveProtocolListener(m_pBasicPrtHandler);
	SAFE_DELETE(m_pBasicPrtHandler);
	SAFE_DELETE(m_pBasicProtocol);
}


/**
 @brief 
 */
void	CServer::OnConnectNetGroupController()
{
	m_pBasicProtocol = new basic::s2c_Protocol();
	RegisterProtocol(m_pBasicProtocol);

	//m_pBasicPrtHandler = new CBasicC2SProtocolHandler(*this);
	AddProtocolListener(m_pBasicPrtHandler);

	EVENT_CONNECT(EVT_CLIENT_JOIN, CServer, CServer::OnClientJoin);
	EVENT_CONNECT(EVT_CLIENT_LEAVE, CServer, CServer::OnClientLeave);

}


/**
 @brief Join Client Event
 */
void CServer::OnClientJoin(CNetEvent &event)
{

}


/**
 @brief Leave Client Event
 */
void CServer::OnClientLeave(CNetEvent &event)
{
	//GroupPtr pGroup = GetRootGroup().GetChildFromUser( event.GetNetId() );
	//RET(!pGroup);

	//m_pBasicProtocol->JoinMember(pGroup->GetId(), SEND_T_V, INVALID_NETID, 
	//	pGroup->GetId(), event.GetNetId() );	
}

