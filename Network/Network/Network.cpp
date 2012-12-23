
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
// ��Ʈ��ũ�� ���õ� Ŭ�������� �ʱ�ȭ �Ѵ�.
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
		return false; // �̹������Ѵٸ� ����

	m_Servers.insert( ServerMap::value_type(pSvr->GetId(), pSvr) );

	// ���� ���ۿ� ���õ� �ڵ� �߰�
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

	// ���� ���� �ڵ� �߰�
	return pSvr->Stop();
}


//------------------------------------------------------------------------
// serverid �� �ش��ϴ� ������ �����Ѵ�.
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
		return false; // �̹� �����Ѵٸ� ����

	m_Clients.insert( ClientMap::value_type(pClt->GetId(), pClt) );

	// Ŭ���̾�Ʈ ���� �ڵ� �߰�
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

	// Ŭ���̾�Ʈ ���� �ڵ� �߰�
	return pClt->Stop();
}


//------------------------------------------------------------------------
// clientId�� �ش��ϴ� Ŭ���̾�Ʈ�� �����Ѵ�.
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

