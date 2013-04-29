
#include "stdafx.h"
#include "Network.h"
#include "Controller/NetController.h"
#include "MultiNetwork/MultiNetwork.h"
#include <MMSystem.h>


using namespace network;

//------------------------------------------------------------------------
// ��Ʈ��ũ�� ���õ� Ŭ�������� �ʱ�ȭ �Ѵ�.
//------------------------------------------------------------------------
bool network::Init(int logicThreadCount, const std::string &svrConfigFileName) // svrConfigFileName=""
{
	srand( timeGetTime() );
	clog::Log( clog::LOG_F_N_O, "Network Init\n" );

	common::InitRandNoDuplicate();
	protocols::Init();

	bool result = CNetController::Get()->Init(logicThreadCount);

	if (result)
	{
		result = multinetwork::CMultiNetwork::Get()->Init( svrConfigFileName );
	}

	return result;
}


//------------------------------------------------------------------------
// �޸� ����
//------------------------------------------------------------------------
void network::Clear()
{
	dbg::Print( "Network Clear" );
	protocols::Cleanup();
	CNetController::Release();
	multinetwork::CMultiNetwork::Release();
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool network::StartServer(int port, ServerBasicPtr pSvr)
{
	clog::Log( clog::LOG_F_N_O, "StartServer port: %d\n", port );
	return CNetController::Get()->StartServer(port, pSvr);
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool network::StopServer(ServerBasicPtr pSvr)
{
	if (!pSvr) return false;
	clog::Log( clog::LOG_F_N_O, "StopServer netid: %d\n", pSvr->GetNetId() );
	return pSvr->Stop();
}

//------------------------------------------------------------------------
// serverid �� �ش��ϴ� ������ �����Ѵ�.
//------------------------------------------------------------------------
ServerBasicPtr network::GetServer(netid serverId)
{
	return CNetController::Get()->GetServer(serverId);
}

//------------------------------------------------------------------------
// Ŭ���̾�Ʈ ����
//------------------------------------------------------------------------
bool network::StartClient(const std::string &ip, int port, ClientBasicPtr pClt)
{
	clog::Log( clog::LOG_F_N_O, "StartClient %s, %d\n", ip.c_str(), port);
	return CNetController::Get()->StartClient(ip, port, pClt);
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool network::StopClient(ClientBasicPtr pClt)
{
	if (!pClt) return false;
	clog::Log( clog::LOG_F_N_O, "StopClient netid: %d\n", pClt->GetNetId() );
	return pClt->Stop();
}


/**
 @brief Connect Delegation to multinetwork 
 */
bool	network::ConnectDelegation( const std::string &linkSvrType, NetGroupDelegationPtr ptr)
{
	return multinetwork::CMultiNetwork::Get()->ConnectDelegation( linkSvrType, ptr );
}


/**
 @brief AddDelegation
 */
bool	network::AddDelegation( const std::string &linkSvrType, multinetwork::CMultiPlugDelegation *ptr)
{
	return multinetwork::CMultiNetwork::Get()->AddDelegation(linkSvrType, ptr);
}


/**
 @brief multinetwork Start
 */
bool	network::StartMultiNetwork()
{
	return multinetwork::CMultiNetwork::Get()->Start();
}


//------------------------------------------------------------------------
// clientId�� �ش��ϴ� Ŭ���̾�Ʈ�� �����Ѵ�.
//------------------------------------------------------------------------
ClientBasicPtr network::GetClient(netid clientId)
{
	return CNetController::Get()->GetClient(clientId);
}
CoreClientPtr network::GetCoreClient(netid clientId)
{
	return CNetController::Get()->GetCoreClient(clientId);
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void network::Proc()
{
	CNetController::Get()->Proc();
}


//------------------------------------------------------------------------
// ��Ʈ����ȯ, ����׿� �ʿ��� ������ ��Ʈ������ ��������.
//------------------------------------------------------------------------
std::string network::ToString()
{
	return CNetController::Get()->ToString();
}
