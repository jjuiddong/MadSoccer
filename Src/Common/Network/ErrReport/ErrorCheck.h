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

	CRemoteClient* CheckClientNetId( ServerBasicPtr pServer, netid clientId, 
		basic::s2c_Protocol *pProtocol, IProtocolDispatcher *pDispatcher );

	CRemoteClient* CheckClientId( ServerBasicPtr pServer, const std::string &id, netid clientId, 
		basic::s2c_Protocol *pProtocol, IProtocolDispatcher *pDispatcher );

	bool CheckClientConnection( CRemoteClient *pClient, 
		basic::s2c_Protocol *pProtocol, IProtocolDispatcher *pDispatcher );


}
