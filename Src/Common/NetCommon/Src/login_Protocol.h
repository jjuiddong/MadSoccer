//------------------------------------------------------------------------
// Name:    D:\Project\GitHub\MadSoccer\Src\Common\NetCommon\Src\login_Protocol.h
// Author:  ProtocolCompiler (by jjuiddong)
// Date:    
//------------------------------------------------------------------------
#pragma once

namespace login {
static const int s2c_Protocol_ID= 100;

class s2c_Protocol : public network::IProtocol
{
public:
	s2c_Protocol() : IProtocol(s2c_Protocol_ID) {}
	void AckLogIn(netid targetId, const network::SEND_FLAG flag, const std::string &id, const int &result);
	void AckLogOut(netid targetId, const network::SEND_FLAG flag, const std::string &id, const int &result);
};
static const int c2s_Protocol_ID= 200;

class c2s_Protocol : public network::IProtocol
{
public:
	c2s_Protocol() : IProtocol(c2s_Protocol_ID) {}
	void ReqLogIn(netid targetId, const network::SEND_FLAG flag, const std::string &id, const std::string &password);
	void ReqLogOut(netid targetId, const network::SEND_FLAG flag, const std::string &id);
};
}
