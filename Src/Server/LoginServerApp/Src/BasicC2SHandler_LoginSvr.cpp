
#include "stdafx.h"
#include "BasicC2SHandler_LoginSvr.h"
#include "Network/ErrReport/ErrorCheck.h"

using namespace network;

CBasicC2SHandler_LoginSvr::CBasicC2SHandler_LoginSvr(multinetwork::CMultiPlug &certifySvr, 
	CServerBasic &svr) :
	network::CBasicC2SHandler(svr)
{
	svr.RegisterProtocol(&m_BasicProtocol);
	certifySvr.RegisterProtocol( &m_CertifyProtocol );
	certifySvr.AddProtocolListener(this);

}

CBasicC2SHandler_LoginSvr::~CBasicC2SHandler_LoginSvr()
{

}


/**
 @brief ReqLogIn
 */
bool CBasicC2SHandler_LoginSvr::ReqLogIn(IProtocolDispatcher &dispatcher,
	netid senderId, const std::string &id, const std::string &passwd, const certify_key &c_key)
{
	if (!CBasicC2SHandler::ReqLogIn(dispatcher, senderId, id, passwd, c_key))
		return false;

	m_CertifyProtocol.ReqUserLogin(SERVER_NETID, SEND_T, id, passwd, "loginsvr");
	return true;
}


/**
 @brief ReqLogOut
 */
bool CBasicC2SHandler_LoginSvr::ReqLogOut(IProtocolDispatcher &dispatcher, netid senderId, const std::string &id)
{

	return true;
}


/**
 @brief ReqMoveToServer
 */
bool CBasicC2SHandler_LoginSvr::ReqMoveToServer(IProtocolDispatcher &dispatcher, 
	netid senderId, const std::string &serverName)
{
	CSession *pClient = network::CheckClientNetId( &GetServer(), senderId, &m_BasicProtocol, &dispatcher );
	RETV(!pClient, false);

	// 유저가 서버 이동이 가능한지 판단~

	// 이동할 서버 검색, 로그인 서버는 lobby Server 로만 이동할 수 있다.
	if (serverName != "lobbysvr" )
	{
		clog::Error( clog::ERROR_PROBLEM, "ReqMoveToServer Error!! invalid servername name=%s", 
			serverName.c_str());
		m_BasicProtocol.AckMoveToServer(senderId, SEND_T, error::ERR_MOVETOSERVER_INVALID_SERVERNAME, serverName, "", 0);
		return false;
	}
	
	MultiPlugPtr pNetGroupCtrl = multinetwork::CMultiNetwork::Get()->GetMultiPlug("lobbysvr");
	if (!pNetGroupCtrl)
	{
		clog::Error( clog::ERROR_PROBLEM, "ReqMoveToServer Error!! not found server group " );
		m_BasicProtocol.AckMoveToServer(senderId, SEND_T, error::ERR_MOVETOSERVER_NOT_FOUND_SERVER, serverName, "", 0);
		return false;
	}
	
	return true;
}


/**
 @brief From CertifyServer 
		인증서버로 부터 받는다.
		ReqLogin -> ReqUserLogin -> AckUserLogin
 */
bool CBasicC2SHandler_LoginSvr::AckUserLogin(IProtocolDispatcher &dispatcher, netid senderId, 
	const error::ERROR_CODE &errorCode, const std::string &id, const certify_key &c_key)
{
	CSession *pClient = network::CheckClientId( &GetServer(), id, 0, &m_BasicProtocol, &dispatcher );
	RETV(!pClient, false);

	if (errorCode != error::ERR_SUCCESS)
	{
		clog::Error( clog::ERROR_PROBLEM, "AckUserId Error!! client generate user id Error id=%s", id.c_str());
		m_BasicProtocol.AckLogIn(pClient->GetNetId(), SEND_T, errorCode, id, 0);
		return false;
	}

	pClient->SetState(SESSIONSTATE_LOGIN); // login state
	pClient->SetCertifyKey( c_key );
	m_BasicProtocol.AckLogIn(pClient->GetNetId(), SEND_T, errorCode, id, c_key );
	return true;
}


/**
 @brief AckUserMoveServer
 */
bool CBasicC2SHandler_LoginSvr::AckUserMoveServer(IProtocolDispatcher &dispatcher, netid senderId, 
	const network::error::ERROR_CODE &errorCode, const std::string &id, const std::string &svrType)
{
	
	return true;
}


/**
 @brief AckUserLogout
 */
bool CBasicC2SHandler_LoginSvr::AckUserLogout(IProtocolDispatcher &dispatcher, netid senderId, 
	const error::ERROR_CODE &errorCode, const std::string &id)
{

	return true;
}
