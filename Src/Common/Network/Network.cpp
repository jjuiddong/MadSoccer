
#include "stdafx.h"
#include "Network.h"
#include "Controller/NetController.h"


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
	dbg::Print( "Network Init" );

	common::InitRandNoDuplicate();
	const bool result = CNetController::Get()->Init(logicThreadCount);

	return result;
}


//------------------------------------------------------------------------
// 메모리 제거
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
	dbg::Print( "StartServer port: %d", port );
	return CNetController::Get()->StartServer(port, pSvr);
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool network::StopServer(ServerBasicPtr pSvr)
{
	if (!pSvr) return false;
	dbg::Print( "StopServer netid: %d", pSvr->GetNetId() );
	return pSvr->Stop();
//	return CNetController::Get()->StopServer(pSvr);
}

//------------------------------------------------------------------------
// serverid 에 해당하는 서버를 리턴한다.
//------------------------------------------------------------------------
ServerBasicPtr network::GetServer(netid serverId)
{
	return CNetController::Get()->GetServer(serverId);
}

//------------------------------------------------------------------------
// 클라이언트 실행
//------------------------------------------------------------------------
bool network::StartClient(const std::string &ip, int port, ClientBasicPtr pClt)
{
	dbg::Print( "StartClient %s, %d", ip.c_str(), port);
	return CNetController::Get()->StartClient(ip, port, pClt);
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool network::StopClient(ClientBasicPtr pClt)
{
	if (!pClt) return false;
	dbg::Print( "StopClient netid: %d", pClt->GetNetId() );
	return pClt->Stop();
//	return CNetController::Get()->StopClient(pClt);
}


//------------------------------------------------------------------------
// clientId에 해당하는 클라이언트를 리턴한다.
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
// 스트링변환, 디버그에 필요한 정보를 스트링으로 내보낸다.
//------------------------------------------------------------------------
std::string network::ToString()
{
	return CNetController::Get()->ToString();
}


//------------------------------------------------------------------------
// Log와 출력창에 동시에 출력한다.
//------------------------------------------------------------------------
void	network::LogNPrint( const char* fmt, ... )
{
	char textString[ 256] = {'\0'};
	va_list args;
	va_start ( args, fmt );
	vsnprintf_s( textString, sizeof(textString), _TRUNCATE, fmt, args );
	va_end ( args );

	clog::Log( textString );
	dbg::Print( textString );
}
