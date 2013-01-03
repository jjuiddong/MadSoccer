//------------------------------------------------------------------------
// Name:    c:\Project\MadSoccer\MadSoccer\Src\Common\NetCommon\login_Protocol.h
// Author:  ProtocolCompiler (by jjuiddong)
// Date:    
//------------------------------------------------------------------------
#pragma once

namespace login {
static const int s2c_Protocol_ID = 100;

class s2c_Protocol : public network::IProtocol
{
public:
	s2c_Protocol() : IProtocol(s2c_Protocol_ID) {}
	void AckLogin(netid targetId, const std::string &id, const int &result);
};
static const int c2s_Protocol_ID = 200;

class c2s_Protocol : public network::IProtocol
{
public:
	c2s_Protocol() : IProtocol(c2s_Protocol_ID) {}
	void ReqLogin(netid targetId, const std::string &id, const std::string &password);
};
}
