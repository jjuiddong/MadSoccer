//------------------------------------------------------------------------
// Name:    S2CPacketListener.h
// Author:  jjuiddong
// Date:    12/23/2012
// 
// 
//------------------------------------------------------------------------
#pragma once

class CS2CProtocolListener : public network::IProtocolListener
{
public:
	virtual void func1(netid senderId) {}
	virtual void func2(netid senderId, std::string &str) {}
	virtual void func3(netid senderId, float value) {}
	virtual void func4(netid senderId) {}
};

