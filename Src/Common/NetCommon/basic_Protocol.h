//------------------------------------------------------------------------
// Name:    c:\Project\MadSoccer\MadSoccer\Src\Common\NetCommon\basic_Protocol.h
// Author:  ProtocolCompiler (by jjuiddong)
// Date:    
//------------------------------------------------------------------------
#pragma once

namespace basic {
static const int s2c_Protocol_ID= 500;

class s2c_Protocol : public network::IProtocol
{
public:
	s2c_Protocol() : IProtocol(s2c_Protocol_ID) {}
	void func1(netid targetId);
	void func2(netid targetId, const std::string &str);
	void func3(netid targetId, const float &value);
	void func4(netid targetId);
	void func5(netid targetId, const std::string &ok, const float &a, const int &b);
};
static const int c2s_Protocol_ID= 600;

class c2s_Protocol : public network::IProtocol
{
public:
	c2s_Protocol() : IProtocol(c2s_Protocol_ID) {}
	void func2(netid targetId, const std::string &str);
	void func3(netid targetId, const float &value);
};
}
