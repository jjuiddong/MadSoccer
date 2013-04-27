
#include "stdafx.h"
#include "Session.h"

using namespace network;


CSession::CSession() : 
	m_Socket(0)
	,	m_State(SESSIONSTATE_DISCONNECT)
	,	m_Id(common::GenerateId()) 
	,	m_P2PState(P2P_CLIENT)
{

}

CSession::~CSession()  
{ 
	closesocket(m_Socket); 
}


/**
 @brief 
 */
void	CSession::ClearConnection()
{
	closesocket(m_Socket);
	m_Socket = 0;
}

