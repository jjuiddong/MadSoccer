//------------------------------------------------------------------------
// Name:    c:\Project\MadSoccer\MadSoccer\Src\Common\NetCommon\login_ProtocolListener.h
// Author:  ProtocolCompiler (by jjuiddong)
// Date:    
//------------------------------------------------------------------------
#pragma once

namespace login {
static const int s2c_ProtocolListener_ID = 100;

class s2c_Dispatcher : public network::IProtocolDispatcher
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
protected:
	virtual void AckLogin(netid senderId, const std::string &id, const int &result){}
};




static const int c2s_ProtocolListener_ID = 200;


class c2s_Dispatcher : public network::IProtocolDispatcher
{
public:
	c2s_Dispatcher();
protected:
	virtual void Dispatch(network::CPacket &packet, const ProtocolListenerList &listeners) override;
};


// ProtocolListener
class c2s_ProtocolListener : virtual public network::IProtocolListener
{
public:
//	c2s_ProtocolListener() : IProtocolListener(c2s_ProtocolListener_ID) {}
	friend class c2s_Dispatcher;
protected:
	virtual void ReqLogin(netid senderId, const std::string &id, const std::string &password){}
};


}
