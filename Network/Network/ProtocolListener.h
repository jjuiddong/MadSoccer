//------------------------------------------------------------------------
// Name:    ProtocolListener.h
// Author:  jjuiddong
// Date:    12/26/2012
// 
// ���������� �޾� ó���ϴ� ������ �Ѵ�.
// Dispatch �Լ����� ���� �������� �Լ����� ȣ���ϰ� �ȴ�.
// ��Ʈ��ũ �������� ���� �����Ϸ��� Dispatch()�� �ҽ����Ϸ� ������.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class IProtocolListener
	{
	public:
		IProtocolListener(int id) : m_Id(id) {}
		virtual ~IProtocolListener() {}
		friend class CTaskLogic;
		friend class CClient;
	protected:
		int m_Id; // �����ϴ� protocol ID �� ������ ���̴�.
	public:
		int GetId() const { return m_Id; }
		virtual void BeforePacketProcess(const CPacket &packet) {} // ��Ŷó���� �����ϱ� ���� ȣ��
		virtual void AfterPacketProcess(const CPacket &packet) {} // ��Ŷ�� ó���� �� ȣ��ȴ�.
	protected:
		virtual void Dispatch(CPacket &packet, const ProtocolListenerList &listeners)=0;
	};
}
