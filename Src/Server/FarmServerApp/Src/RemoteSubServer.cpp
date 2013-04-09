
#include "stdafx.h"
#include "RemoteSubServer.h"

using namespace network;

CRemoteSubServer::CRemoteSubServer() : 
	m_OuterPort(0)
{

}


/**
 @brief SetP2PLink
 */
void CRemoteSubServer::SetP2PLink( const std::vector<std::string> &v )
{
	m_P2PLink.clear();
	BOOST_FOREACH(auto &svrType, v)
	{
		SLinkState link;
		link.m_SvrType = svrType;
		link.m_State = SLinkState::WAIT;
		link.m_Type = network::CLIENT; // default
		m_P2PLink.push_back( link );
	}
}


/**
 @brief SetInputLink
 */
void	CRemoteSubServer::SetInputLink( const std::vector<std::string> &v )
{
	m_InputLink.clear();
	BOOST_FOREACH(auto &svrType, v)
	{
		SLinkState link;
		link.m_SvrType = svrType;
		link.m_State = SLinkState::WAIT;
		link.m_Type = network::CLIENT;
		m_InputLink.push_back( link );
	}
}


/**
 @brief SetOutputLink
 */
void	CRemoteSubServer::SetOutputLink( const std::vector<std::string> &v )
{
	m_OutputLink.clear();
	BOOST_FOREACH(auto &svrType, v)
	{
		SLinkState link;
		link.m_SvrType = svrType;
		link.m_State = SLinkState::WAIT;
		link.m_Type = network::SERVER;
		m_OutputLink.push_back( link );
	}
}


/**
 @brief GetLinkInfo
 */
void	CRemoteSubServer::GetLinkInfo( const std::string &linkSvrType, OUT std::vector<SHostInfo> &v )
{
	GetLinkInfo( m_OutputLink, linkSvrType, v );
	GetLinkInfo( m_P2PLink, linkSvrType, v );
}


/**
 @brief GetLinkInfo
 */
void	CRemoteSubServer::GetLinkInfo( const Links &linkServers,  const std::string &linkSvrType, OUT std::vector<SHostInfo> &v )
{
	BOOST_FOREACH(auto &link, linkServers)
	{
		if (link.m_SvrType == linkSvrType &&
			link.m_State == SLinkState::CONNECT &&
			link.m_Type == SERVER)
		{
			SHostInfo info;
			info.ip = link.m_ServerIp;
			info.portnum = link.m_ServerPort;
			v.push_back(info);
		}
	}
}


/**
 @brief GetToBindInnerPort
  ���ڷ� �Ѿ�� port �� �̹� Bind ���̸� true �� �����ϰ�, �׷��� �ʴٸ�
  false�� �����Ѵ�.
 */
bool CRemoteSubServer::IsBindPort(const Links &linkServers, int port)
{
	BOOST_FOREACH(auto &link, linkServers)
	{
		if (link.m_Type == SERVER &&
			link.m_ServerPort == port)
		{
			return true;
		}
	}
	return false;
}


/**
 @brief GetToBindInnerPort
 */
int CRemoteSubServer::GetToBindInnerPort(int basePort)
{
	int bindPort = basePort;
	for (; bindPort < basePort+100; ++bindPort)
	{
		if (IsBindPort( m_OutputLink, bindPort))
			continue;
		if (IsBindPort( m_P2PLink, bindPort))
			continue;
		break;
	}
	return bindPort;
}


/**
 @brief GetToBindOuterPort
 ���� outerport ���� �ϳ��� �����ϰ� �ȴ�.
 */
int CRemoteSubServer::GetToBindOuterPort(int basePort)
{
	if (m_OuterPort == basePort)
		return m_OuterPort + 1;
	return basePort;
}


/**
 @brief SetInnerBindPort
 */
void	CRemoteSubServer::SetInnerBindPort( const std::string &linkSvrType, int bindPort )
{
	SLinkState *p = GetLinkState(SERVER, linkSvrType);
	if (!p)
		return;

	p->m_ServerIp = GetIp();
	p->m_ServerPort = bindPort;	
}


/**
 @brief SetOuterBindPort
 */
void	CRemoteSubServer::SetOuterBindPort( const std::string &linkSvrType, int bindPort )
{
	m_OuterPort = bindPort;
}


/**
 @brief GetLinkState
 */
SLinkState* CRemoteSubServer::GetLinkState( SERVICE_TYPE type, const std::string &linkSvrType )
{
	SLinkState *pRet = GetLinkState( m_InputLink, type, linkSvrType );
	if (pRet) return pRet;

	pRet = GetLinkState( m_OutputLink, type, linkSvrType );
	if (pRet) return pRet;

	pRet = GetLinkState( m_P2PLink, type, linkSvrType );
	if (pRet) return pRet;

	return NULL;
}


/**
 @brief GetLinkState
 */
SLinkState* CRemoteSubServer::GetLinkState( Links &links, SERVICE_TYPE type, const std::string &linkSvrType )
{
	BOOST_FOREACH(auto &svr, links)
	{
		if ((svr.m_Type == type) &&
			(svr.m_SvrType == linkSvrType))
		{
			return &svr;
		}
	}
	return NULL;
}
