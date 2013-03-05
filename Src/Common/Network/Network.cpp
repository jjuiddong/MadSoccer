
#include "stdafx.h"
#include "Network.h"
#include "Controller/NetController.h"


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
	dbg::Print( "Network Init" );

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
bool network::StartServer(int port, CServer *pSvr)
{
	dbg::Print( "StartServer port: %d", port );
	return CNetController::Get()->StartServer(port, pSvr);
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool network::StopServer(CServer *pSvr)
{
	if (!pSvr) return false;
	dbg::Print( "StopServer netid: %d", pSvr->GetNetId() );
	return pSvr->Stop();
//	return CNetController::Get()->StopServer(pSvr);
}

//------------------------------------------------------------------------
// serverid �� �ش��ϴ� ������ �����Ѵ�.
//------------------------------------------------------------------------
ServerPtr network::GetServer(netid serverId)
{
	return CNetController::Get()->GetServer(serverId);
}

//------------------------------------------------------------------------
// Ŭ���̾�Ʈ ����
//------------------------------------------------------------------------
bool network::StartClient(const std::string &ip, int port, CClient *pClt)
{
	dbg::Print( "StartClient %s, %d", ip.c_str(), port);
	return CNetController::Get()->StartClient(ip, port, pClt);
}

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool network::StopClient(CClient *pClt)
{
	if (!pClt) return false;
	dbg::Print( "StopClient netid: %d", pClt->GetNetId() );
	return pClt->Stop();
//	return CNetController::Get()->StopClient(pClt);
}


//------------------------------------------------------------------------
// clientId�� �ش��ϴ� Ŭ���̾�Ʈ�� �����Ѵ�.
//------------------------------------------------------------------------
ClientPtr network::GetClient(netid clientId)
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


//------------------------------------------------------------------------
// Log�� ���â�� ���ÿ� ����Ѵ�.
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
