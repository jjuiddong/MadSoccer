//------------------------------------------------------------------------
// Name:    Src\basic_ProtocolListener.h
// Author:  ProtocolCompiler (by jjuiddong)
// Date:    
//------------------------------------------------------------------------
#pragma once

#include "basic_ProtocolData.h"

namespace basic {

using namespace network;
using namespace marshalling;
static const int s2c_Dispatcher_ID = 200;

// Protocol Dispatcher
class s2c_Dispatcher: public network::IProtocolDispatcher
{
public:
	s2c_Dispatcher();
protected:
	virtual bool Dispatch(network::CPacket &packet, const ProtocolListenerList &listeners) override;
};


// ProtocolListener
class s2c_ProtocolListener : virtual public network::IProtocolListener
{
	friend class s2c_Dispatcher;
	virtual bool func1(basic::func1_Packet &packet) { return true; }
	virtual bool func2(basic::func2_Packet &packet) { return true; }
	virtual bool func3(basic::func3_Packet &packet) { return true; }
	virtual bool func4(basic::func4_Packet &packet) { return true; }
};


static const int c2s_Dispatcher_ID = 300;

// Protocol Dispatcher
class c2s_Dispatcher: public network::IProtocolDispatcher
{
public:
	c2s_Dispatcher();
protected:
	virtual bool Dispatch(network::CPacket &packet, const ProtocolListenerList &listeners) override;
};


// ProtocolListener
class c2s_ProtocolListener : virtual public network::IProtocolListener
{
	friend class c2s_Dispatcher;
	virtual bool func2(basic::func2_Packet &packet) { return true; }
	virtual bool func3(basic::func3_Packet &packet) { return true; }
};


}
