//------------------------------------------------------------------------
// Name:    c:\Project\GitHub\MadSoccer\Src\Common\NetCommon\basic_ProtocolListener.h
// Author:  ProtocolCompiler (by jjuiddong)
// Date:    
//------------------------------------------------------------------------
#pragma once

namespace basic {
static const int s2c_Dispatcher_ID = 500;

// Protocol Dispatcher
class s2c_Dispatcher: public network::IProtocolDispatcher
{
public:
	s2c_Dispatcher();
protected:
	virtual void Dispatch(network::CPacket &packet, const ProtocolListenerList &listeners) override;
};


// ProtocolListener
class s2c_ProtocolListener : virtual public network::IProtocolListener
{
	friend class s2c_Dispatcher;
	virtual void func1(netid senderId){}
	virtual void func2(netid senderId, const std::string &str){}
	virtual void func3(netid senderId, const float &value){}
	virtual void func4(netid senderId){}
	virtual void func5(netid senderId, const std::string &ok, const float &a, const int &b){}
};


static const int c2s_Dispatcher_ID = 600;

// Protocol Dispatcher
class c2s_Dispatcher: public network::IProtocolDispatcher
{
public:
	c2s_Dispatcher();
protected:
	virtual void Dispatch(network::CPacket &packet, const ProtocolListenerList &listeners) override;
};


// ProtocolListener
class c2s_ProtocolListener : virtual public network::IProtocolListener
{
	friend class c2s_Dispatcher;
	virtual void func2(netid senderId, const std::string &str){}
	virtual void func3(netid senderId, const float &value){}
};


}
