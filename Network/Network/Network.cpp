
#include "stdafx.h"
#include "Network.h"
#include "Server.h"
#include "Client.h"



namespace network
{
	typedef std::list<CServer*> ServerList;
	typedef std::list<CClient*> ClientList;
	typedef ServerList::iterator ServerItor;
	typedef ClientList::iterator ClientItor;

	ServerList m_ServerList;
	ClientList m_ClientList;

};

using namespace network;




//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool network::StartServer(int port, CServer *pSvr)
{
	if (!pSvr)
		return false;

	m_ServerList.push_back( pSvr );

	// ���� ���ۿ� ���õ� �ڵ� �߰�
	return pSvr->Start(port);
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool network::StopServer(CServer *pSvr)
{
	if (!pSvr)
		return false;

	ServerItor it = find_if( m_ServerList.begin(), m_ServerList.end(), IsServer(pSvr) );
	if (m_ServerList.end() != it)
		return false;

	m_ServerList.remove( pSvr );

	// ���� ���� �ڵ� �߰�
	return pSvr->Stop();
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool network::StartClient(std::string ip, int port, CClient *pClt)
{
	if (!pClt)
		return false;

	m_ClientList.push_back( pClt );

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

	ClientItor it = find_if( m_ClientList.begin(), m_ClientList.end(), IsClient(pClt) );
	if (m_ClientList.end() != it)
		return false;

	m_ClientList.remove( pClt );

	// Ŭ���̾�Ʈ ���� �ڵ� �߰�
	return pClt->Stop();
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void network::Proc()
{
	ClientItor cit = m_ClientList.begin();
	while (m_ClientList.end() != cit)
	{
		CClient *pclt = *cit++;
		pclt->Proc();
	}
}

