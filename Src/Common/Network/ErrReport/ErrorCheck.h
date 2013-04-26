/**
Name:   ErrorCheck.h
Author:  jjuiddong
Date:    4/22/2013

	에러 체크를 처리하는 함수들 모음
*/
#pragma once

namespace basic { class s2c_Protocol; }

namespace network
{

	CSession* CheckClientNetId( ServerBasicPtr pServer, netid clientId, 
		basic::s2c_Protocol *pProtocol, IProtocolDispatcher *pDispatcher );

	CSession* CheckClientId( ServerBasicPtr pServer, const std::string &id, netid clientId, 
		basic::s2c_Protocol *pProtocol, IProtocolDispatcher *pDispatcher );

	bool CheckClientConnection( CSession *pClient, 
		basic::s2c_Protocol *pProtocol, IProtocolDispatcher *pDispatcher );


}
