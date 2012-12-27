//------------------------------------------------------------------------
// Name:    c2s_Protocol.h
// Author:  ProtocolCompiler (by jjuiddong)
// Date:    
//------------------------------------------------------------------------
#pragma once

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


// Protocol
class c2s_Protocol : public network::IProtocol
{
public:
	c2s_Protocol() : IProtocol(c2s_ProtocolListener_ID) {}
	void func2(netid targetId, const std::string &str);
	void func3(netid targetId, const float &value);
};
