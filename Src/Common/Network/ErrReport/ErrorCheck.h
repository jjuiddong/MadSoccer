/**
Name:   ErrorCheck.h
Author:  jjuiddong
Date:    4/22/2013

	���� üũ�� ó���ϴ� �Լ��� ����
*/
#pragma once

namespace basic { class s2c_Protocol; }

namespace network
{

	CRemoteClient* CheckClient( ServerBasicPtr pServer, basic::s2c_Protocol *pProtocol, netid clientId );
	CRemoteClient* CheckClient( ServerBasicPtr pServer, basic::s2c_Protocol *pProtocol, const std::string &id, netid clientId );

}
