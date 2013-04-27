
#include "stdafx.h"
#include "BasicC2SProtocolHandler_LoginSvr.h"
#include "Network/ErrReport/ErrorCheck.h"

using namespace network;

CBasicC2SProtocolHandler_LoginSvr::CBasicC2SProtocolHandler_LoginSvr(multinetwork::CNetGroupController &certifySvr, 
	CServerBasic &svr) :
	network::CBasicC2SProtocolHandler(svr)
{
	svr.RegisterProtocol(&m_BasicProtocol);
	certifySvr.RegisterProtocol( &m_CertifyProtocol );
	certifySvr.AddProtocolListener(this);

}

CBasicC2SProtocolHandler_LoginSvr::~CBasicC2SProtocolHandler_LoginSvr()
{

}


/**
 @brief ReqLogIn
 */
bool CBasicC2SProtocolHandler_LoginSvr::ReqLogIn(IProtocolDispatcher &dispatcher,
	netid senderId, const std::string &id, const std::string &passwd)
{
	if (!CBasicC2SProtocolHandler::ReqLogIn(dispatcher, senderId, id, passwd))
		return false;

	m_CertifyProtocol.ReqUserId(SERVER_NETID, SEND_T, id, passwd);
	return true;
}


/**
 @brief ReqLogOut
 */
bool CBasicC2SProtocolHandler_LoginSvr::ReqLogOut(IProtocolDispatcher &dispatcher, netid senderId, const std::string &id)
{

	return true;
}


/**
 @brief ReqMoveToServer
 */
bool CBasicC2SProtocolHandler_LoginSvr::ReqMoveToServer(IProtocolDispatcher &dispatcher, 
	netid senderId, const std::string &serverName)
{
	CSession *pClient = network::CheckClientNetId( &GetServer(), senderId, &m_BasicProtocol, &dispatcher );
	RETV(!pClient, false);

	// ������ ���� �̵��� �������� �Ǵ�~

	// �̵��� ���� �˻�, �α��� ������ lobby Server �θ� �̵��� �� �ִ�.
	if (serverName != "lobbysvr" )
	{
		clog::Error( clog::ERROR_PROBLEM, "ReqMoveToServer Error!! invalid servername name=%s", 
			serverName.c_str());
		m_BasicProtocol.AckMoveToServer(senderId, SEND_T, error::ERR_MOVETOSERVER_INVALID_SERVERNAME, serverName);
		return false;
	}
	
	NetGroupControllerPtr pNetGroupCtrl = multinetwork::CMultiNetwork::Get()->GetController("lobbysvr");
	if (!pNetGroupCtrl)
	{
		clog::Error( clog::ERROR_PROBLEM, "ReqMoveToServer Error!! not found server group " );
		m_BasicProtocol.AckMoveToServer(senderId, SEND_T, error::ERR_MOVETOSERVER_NOT_FOUND_SERVER, serverName);
		return false;
	}
	
	
	return true;
}


/**
 @brief ���������� ���� �޴´�.
		ReqLogin �� �޴´�.
 */
bool CBasicC2SProtocolHandler_LoginSvr::AckUserId(IProtocolDispatcher &dispatcher, netid senderId, 
	const error::ERROR_CODE &errorCode, const std::string &id, const netid &userId)
{
	CSession *pClient = network::CheckClientId( &GetServer(), id, userId, &m_BasicProtocol, &dispatcher );
	RETV(!pClient, false);

	if (errorCode != error::ERR_SUCCESS)
	{
		clog::Error( clog::ERROR_PROBLEM, "AckUserId Error!! client generate user id Error id=%s", id.c_str());
		return false;
	}

	pClient->SetState(SESSIONSTATE_LOGIN); // login state
	m_BasicProtocol.AckLogIn(pClient->GetNetId(), SEND_T, errorCode, id, userId);
	return true;
}

