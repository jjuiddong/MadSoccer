//------------------------------------------------------------------------
// Name:    C2SProtocol.h
// Author:  jjuiddong
// Date:    12/25/2012
// 
// 테스트 코드
//------------------------------------------------------------------------
#pragma once

static const int C2SProtocol_ID = 2;

class C2SProtocol : public network::IProtocol
{
public:
	C2SProtocol() : IProtocol(C2SProtocol_ID) {}
	void func(netid targetId, const std::string &str);
};


class C2SProtocolListener : public network::IProtocolListener
{
public:
	C2SProtocolListener() : IProtocolListener(C2SProtocol_ID) {}
	virtual void func1(netid senderId) {}
	virtual void func2(netid senderId, std::string &str) {}
	virtual void func3(netid senderId, float value) {}
	virtual void func4(netid senderId) {}

protected:
	virtual void Dispatch(network::CPacket &packet, const ProtocolListenerList &listeners) override;
};
