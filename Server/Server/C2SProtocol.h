//------------------------------------------------------------------------
// Name:    C2SProtocol.h
// Author:  jjuiddong
// Date:    12/25/2012
// 
// �׽�Ʈ �ڵ�
//------------------------------------------------------------------------
#pragma once

class CC2SProtocol : public network::IProtocol
{
protected:
	virtual void Dispatch(network::CPacket &packet, const ListenerList &listeners) override;

public:
	void func(netid targetId, const std::string &str);
};
