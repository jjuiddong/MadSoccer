//------------------------------------------------------------------------
// Name:    C:\GitHub\MadSoccer\Src\Common\NetProtocol\Src\login_ProtocolListener.h
// Author:  ProtocolCompiler (by jjuiddong)
// Date:    
//------------------------------------------------------------------------
#pragma once

namespace login {
static const int s2c_Dispatcher_ID = 100;

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
	virtual void AckLogIn(netid senderId, const std::string &id, const int &result){}
	virtual void AckLogOut(netid senderId, const std::string &id, const int &result){}
};


static const int c2s_Dispatcher_ID = 200;

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
	virtual void ReqLogIn(netid senderId, const std::string &id, const std::string &password){}
	virtual void ReqLogOut(netid senderId, const std::string &id){}
};


}
