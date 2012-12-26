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
		friend class CTaskLogic;
		friend class CClient;
	protected:
		int m_Id;
		NetObjectPtr m_pNetObject;
	public:
		void SetId(int id) { m_Id = id; }
		void SetNetObject(NetObjectPtr pServer) { m_pNetObject = pServer; }
		const NetObjectPtr& GetNetObject() { return m_pNetObject; }

	protected:
		virtual void Dispatch(CPacket &packet, const ListenerList &listeners)=0;
	};
}
