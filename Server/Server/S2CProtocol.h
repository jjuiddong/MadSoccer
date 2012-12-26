//------------------------------------------------------------------------
// Name:    S2CPacketDispatcher.h
// Author:  jjuiddong
// Date:    12/23/2012
// 
// 테스트 코드
// Server to Client 프로토콜 정의
//------------------------------------------------------------------------
#pragma once

class CS2CProtocol : public network::IProtocol
{
protected:
	virtual void Dispatch(network::CPacket &packet, const ListenerList &listeners) override;

public:
	void func1(netid targetId);
	void func2(netid targetId, const std::string &str);
	void func3(netid targetId, float value);
	void func4(netid targetId);
};

