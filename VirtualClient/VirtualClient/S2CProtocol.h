//------------------------------------------------------------------------
// Name:    S2CPacketDispatcher.h
// Author:  jjuiddong
// Date:    12/23/2012
// 
// 테스트 코드
// Server to Client 프로토콜 정의
//------------------------------------------------------------------------
#pragma once

static const int S2CProtocol_ID = 1;

class S2CProtocol : public network::IProtocol
{
public:
	S2CProtocol() : IProtocol(S2CProtocol_ID) {}
	void func1(netid targetId);
	void func2(netid targetId, const std::string &str);
	void func3(netid targetId, float value);
	void func4(netid targetId);
};


class S2CProtocolListener : public network::IProtocolListener
{
public:
	S2CProtocolListener() : IProtocolListener(S2CProtocol_ID) {}
	virtual void func1(netid senderId) {}
	virtual void func2(netid senderId, std::string &str) {}
	virtual void func3(netid senderId, float value) {}
	virtual void func4(netid senderId) {}

protected:
	virtual void Dispatch(network::CPacket &packet, const ProtocolListenerList &listeners) override;
};

