
#include "stdafx.h"
#include "Network.h"
#include "Server.h"
#include "Client.h"



namespace network
{
	typedef std::map<int, CServer*> ServerMap;
	typedef std::map<int, CClient*> ClientMap;
	typedef ServerMap::iterator ServerItor;
	typedef ClientMap::iterator ClientItor;

	ServerMap m_Servers;
	ClientMap m_Clients;

};

using namespace network;


//------------------------------------------------------------------------
// 네트워크에 관련된 클래스들을 초기화 한다.
//------------------------------------------------------------------------
bool network::Init()
{
	common::InitRandNoDuplicate();

	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool network::StartServer(int port, CServer *pSvr)
{
	if (!pSvr)
		return false;

	ServerItor it = m_Servers.find(pSvr->GetId());
	if (m_Servers.end() != it)
		return false; // 이미존재한다면 실패

	m_Servers.insert( ServerMap::value_type(pSvr->GetId(), pSvr) );

	// 서버 시작에 관련된 코드 추가
	error::Log( common::format("%d Server Start", pSvr->GetId()) );

	return pSvr->Start(port);
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool network::StopServer(CServer *pSvr)
{
	if (!pSvr)
		return false;

	ServerItor it = m_Servers.find(pSvr->GetId());
	if (m_Servers.end() == it)
		return false;

	m_Servers.erase(it);

	// 서버 종료 코드 추가
	return pSvr->Stop();
}


//------------------------------------------------------------------------
// serverid 에 해당하는 서버를 리턴한다.
//------------------------------------------------------------------------
CServer* network::GetServer(int serverId)
{
	ServerItor it = m_Servers.find(serverId);
	if (m_Servers.end() != it)
		return NULL;

	return it->second;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool network::StartClient(std::string ip, int port, CClient *pClt)
{
	if (!pClt)
		return false;

	ClientItor it = m_Clients.find(pClt->GetId());
	if (m_Clients.end() != it)
		return false; // 이미 존재한다면 실패

	m_Clients.insert( ClientMap::value_type(pClt->GetId(), pClt) );

	// 클라이언트 접속 코드 추가
	return pClt->Start(ip, port);
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool network::StopClient(CClient *pClt)
{
	if (!pClt)
		return false;

	ClientItor it = m_Clients.find(pClt->GetId());
	if (m_Clients.end() == it)
		return false;

	m_Clients.erase(it);

	// 클라이언트 종료 코드 추가
	return pClt->Stop();
}


//------------------------------------------------------------------------
// clientId에 해당하는 클라이언트를 리턴한다.
//------------------------------------------------------------------------
CClient* network::GetClient(int clientId)
{
	ClientItor it = m_Clients.find(clientId);
	if (m_Clients.end() == it)
		return NULL;
	return it->second;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void network::Proc()
{
	ClientItor cit = m_Clients.begin();
	while (m_Clients.end() != cit)
	{
		CClient *pclt = cit++->second;
		pclt->Proc();
	}
}

