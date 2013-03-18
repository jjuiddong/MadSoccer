//------------------------------------------------------------------------
// Name:    D:\Project\GitHub\MadSoccer\Src\Common\NetCommon\Src\basic_ProtocolListener.h
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
	virtual void AckGroupList(netid senderId, const int &errorCode, const GroupVector &groups){}
	virtual void AckGroupJoin(netid senderId, const int &errorCode){}
	virtual void AckGroupCreate(netid senderId, const int &errorCode, const std::string &groupName, const netid &groupid){}
	virtual void AckP2PConnect(netid senderId, const int &errorCode, const network::P2P_STATE &state, const std::string &ip, const int &port){}
	virtual void AckP2PHostCreate(netid senderId, const int &errorCode){}
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
	virtual void ReqGroupList(netid senderId, const netid &groupid){}
	virtual void ReqGroupJoin(netid senderId, const netid &groupid){}
	virtual void ReqGroupCreate(netid senderId, const netid &parentGroupId, const std::string &groupName){}
	virtual void ReqP2PConnect(netid senderId){}
	virtual void ReqP2PHostCreate(netid senderId, const bool &isSuccess){}
	virtual void func2(netid senderId, const std::string &str){}
	virtual void func3(netid senderId, const float &value){}
};


}
