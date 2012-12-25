
#include "stdafx.h"
#include "Network.h"
#include "NetController.h"

namespace network
{
	// 아직아무것도없음
};

using namespace network;


//------------------------------------------------------------------------
// 네트워크에 관련된 클래스들을 초기화 한다.
//------------------------------------------------------------------------
bool network::Init(int logicThreadCount)
{
	common::InitRandNoDuplicate();
	const bool result = CNetController::Get()->Init(logicThreadCount);

	return result;
}


//------------------------------------------------------------------------
// 메모리 제거
//------------------------------------------------------------------------
void network::Clear()
{
	CNetController::Release();

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool network::StartServer(int port, CServer *pSvr)
{
	return CNetController::Get()->StartServer(port, pSvr);
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool network::StopServer(CServer *pSvr)
{
	return CNetController::Get()->StopServer(pSvr);
}

//------------------------------------------------------------------------
// serverid 에 해당하는 서버를 리턴한다.
//------------------------------------------------------------------------
CServer* network::GetServer(int serverId)
{
	return CNetController::Get()->GetServer(serverId);
}

//------------------------------------------------------------------------
// 클라이언트 실행
//------------------------------------------------------------------------
bool network::StartClient(const std::string &ip, int port, CClient *pClt)
{
	return CNetController::Get()->StartClient(ip, port, pClt);
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool network::StopClient(CClient *pClt)
{
	return CNetController::Get()->StopClient(pClt);
}


//------------------------------------------------------------------------
// clientId에 해당하는 클라이언트를 리턴한다.
//------------------------------------------------------------------------
CClient* network::GetClient(int clientId)
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

