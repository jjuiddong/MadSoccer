//------------------------------------------------------------------------
// Name:    basic_ProtocolListener.h
// Author:  ProtocolCompiler (by jjuiddong)
// Date:    
//------------------------------------------------------------------------
#pragma once

namespace basic {
static const int s2c_ProtocolListener_ID = 200;

// ProtocolListener
class s2c_ProtocolListener : public network::IProtocolListener
{
public:
	s2c_ProtocolListener() : IProtocolListener(s2c_ProtocolListener_ID) {}
protected:
	virtual void Dispatch(network::CPacket &packet, const ProtocolListenerList &listeners) override;
	virtual void func1(netid senderId){}
	virtual void func2(netid senderId, const std::string &str){}
	virtual void func3(netid senderId, const float &value){}
	virtual void func4(netid senderId){}
};


static const int c2s_ProtocolListener_ID = 300;

// ProtocolListener
class c2s_ProtocolListener : public network::IProtocolListener
{
public:
	c2s_ProtocolListener() : IProtocolListener(c2s_ProtocolListener_ID) {}
protected:
	virtual void Dispatch(network::CPacket &packet, const ProtocolListenerList &listeners) override;
	virtual void func2(netid senderId, const std::string &str){}
	virtual void func3(netid senderId, const float &value){}
};


}
