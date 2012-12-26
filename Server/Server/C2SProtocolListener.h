//------------------------------------------------------------------------
// Name:    C2SPacketListener.h
// Author:  jjuiddong
// Date:    12/25/2012
// 
// 테스트 코드
//------------------------------------------------------------------------
#pragma once

class CC2SProtocolListener : public network::IProtocolListener
{
public:
	virtual void func1(netid senderId) {}
	virtual void func2(netid senderId, std::string &str) {}
	virtual void func3(netid senderId, float value) {}
	virtual void func4(netid senderId) {}
};
