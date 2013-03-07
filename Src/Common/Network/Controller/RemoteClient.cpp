
#include "stdafx.h"
#include "RemoteClient.h"

using namespace network;


CRemoteClient::CRemoteClient() : 
	m_Socket(0)
,	m_Id(common::GenerateId()) 
{
}

CRemoteClient::~CRemoteClient()  
{ 
	closesocket(m_Socket); 
}
