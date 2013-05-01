// LoginServerApp.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "LoginServerApp.h"
#include "Src/LoginServer.h"
#include "Network/Service/ServerApp.h"

using namespace network;


// CLoginServerApp
class CLoginServerApp : public network::CServerApp
{
public:
	CLoginServerApp();
	virtual ~CLoginServerApp();
	virtual bool OnInit();

	CLoginServer *m_pLoginSvr;
};

DECLARE_SERVERAPP(CLoginServerApp);
IMPLEMENT_SERVERAPP(CLoginServerApp);

CLoginServerApp::CLoginServerApp() :
	m_pLoginSvr(NULL)
{
	m_MemoryMonitorFilePath = "loginserver_monitor.json";
	m_NetworkConfigFilePath  = "loginserver_config.json";
	m_TitleName = "LoginServer";
	SetRect(&m_WindowRect, 0, 300, 200, 300);
}

CLoginServerApp::~CLoginServerApp()
{
	SAFE_DELETE(m_pLoginSvr);
}


/**
 @brief 
 */
bool CLoginServerApp::OnInit()
{
	m_pLoginSvr = new CLoginServer();
	if (!network::ConnectDelegation("client", 	m_pLoginSvr))
	{
		clog::Error( clog::ERROR_CRITICAL, "network :: ConnectDelegation Fail !!\n" );
		return false;
	}
	if (!network::AddDelegation( "lobbysvr", new network::CSubServerConnector()))
	{
		clog::Error( clog::ERROR_CRITICAL, "network :: AddDelegation Fail !!\n" );
		return false;
	}
	return true;
}
