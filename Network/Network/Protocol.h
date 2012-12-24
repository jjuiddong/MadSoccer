//------------------------------------------------------------------------
// Name:    Protocol.h
// Author:  jjuiddong
// Date:    12/23/2012
// 
// ������ Ŭ���̾�Ʈ���� �ְ�޴� ���������� �����Ѵ�.
// ��Ʈ��ũ �������� ���� �����Ϸ��� Dispatch()�� �ҽ����Ϸ� ������.
// ������ Ŭ���̾�Ʈ���� ���������� *.prt ���Ͽ� �����ؾ� �Ѵ�.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class IProtocol
	{
	public:
		virtual void Dispatch(const CPacket &packet, const ListenerList &listeners)=0;
	};
}
