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
	void func2(netid targetId, const std::string &str);
	void func3(netid targetId, float value);
};


class C2SProtocolListener : public network::IProtocolListener
{
public:
	C2SProtocolListener() : IProtocolListener(C2SProtocol_ID) {}
protected:
	virtual void Dispatch(network::CPacket &packet, const ProtocolListenerList &listeners) override;
	virtual void func2(netid senderId, std::string &str) {}
	virtual void func3(netid senderId, float value) {}
};
