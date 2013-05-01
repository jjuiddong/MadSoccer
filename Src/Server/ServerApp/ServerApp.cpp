// ServerApp.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "ServerApp.h"

#include "NetProtocol/Src/login_Protocol.h"
#include "NetProtocol/Src/login_ProtocolListener.h"
#include "NetProtocol/Src/login_Protocol.cpp"
#include "NetProtocol/Src/login_ProtocolListener.cpp"

#include "NetProtocol/Src/basic_Protocol.h"
#include "NetProtocol/Src/basic_ProtocolListener.h"

#include "Lib/ChatServer.h"
#include "Lib/ServerLauncher.h"
#include "Lib/LobbyServer.h"

#include "Network/Service/ServerApp.h"


// CLobbyServerApp
class CLobbyServerApp : public CServerApp
{
public:
	CLobbyServerApp();
	virtual ~CLobbyServerApp();
	virtual bool OnInit() override;
	virtual std::string ToString() override;

	CLobbyServer *m_pLobbyServer;
};

DECLARE_SERVERAPP(CLobbyServerApp);
IMPLEMENT_SERVERAPP(CLobbyServerApp);

CLobbyServerApp::CLobbyServerApp() :
	m_pLobbyServer(NULL)
{
	m_MemoryMonitorFilePath = "madsoccer_server_monitor.json" ;
	m_NetworkConfigFilePath  = "madsoccer_server_config.json";
	m_TitleName = "LobbyServer";
	SetRect(&m_WindowRect, 400, 0, 200, 300);
}

CLobbyServerApp::~CLobbyServerApp()
{
	SAFE_DELETE(m_pLobbyServer);
}

std::string CLobbyServerApp::ToString()
{
	if (m_pLobbyServer)
		return m_pLobbyServer->ToString();
	return "";
}


/**
 @brief 
 */
bool CLobbyServerApp::OnInit()
{
	m_pLobbyServer = new CLobbyServer();
	if (!network::ConnectDelegation( "client", m_pLobbyServer ))
	{
		clog::Error(log::ERROR_CRITICAL, "network::ConnectDelegation() fail !!\n" );
		return false;
	}
	return true;
}
