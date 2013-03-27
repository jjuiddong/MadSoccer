
#include "stdafx.h"
#include "Network.h"
#include "Controller/NetController.h"
#include <MMSystem.h>


namespace network
{
	// �����ƹ��͵�����
};

using namespace network;


//------------------------------------------------------------------------
// ��Ʈ��ũ�� ���õ� Ŭ�������� �ʱ�ȭ �Ѵ�.
//------------------------------------------------------------------------
bool network::Init(int logicThreadCount)
{
	srand( timeGetTime() );
	clog::Log( clog::LOG_F_N_O, "Network Init" );

	common::InitRandNoDuplicate();
	const bool result = CNetController::Get()->Init(logicThreadCount);
	return result;
}


//------------------------------------------------------------------------
// �޸� ����
//------------------------------------------------------------------------
void network::Clear()
{
	dbg::Print( "Network Clear" );
	CNetController::Release();
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool network::StartServer(int port, ServerBasicPtr pSvr)
{
	clog::Log( clog::LOG_F_N_O, "StartServer port: %d", port );
	return CNetController::Get()->StartServer(port, pSvr);
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool network::StopServer(ServerBasicPtr pSvr)
{
	if (!pSvr) return false;
	clog::Log( clog::LOG_F_N_O, "StopServer netid: %d", pSvr->GetNetId() );
	return pSvr->Stop();
//	return CNetController::Get()->StopServer(pSvr);
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
	clog::Log( clog::LOG_F_N_O, "StartClient %s, %d", ip.c_str(), port);
	return CNetController::Get()->StartClient(ip, port, pClt);
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool network::StopClient(ClientBasicPtr pClt)
{
	if (!pClt) return false;
	clog::Log( clog::LOG_F_N_O, "StopClient netid: %d", pClt->GetNetId() );
	return pClt->Stop();
//	return CNetController::Get()->StopClient(pClt);
}


//------------------------------------------------------------------------
// clientId�� �ش��ϴ� Ŭ���̾�Ʈ�� �����Ѵ�.
//------------------------------------------------------------------------
ClientBasicPtr network::GetClient(netid clientId)
{
	return CNetController::Get()->GetClient(clientId);
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
