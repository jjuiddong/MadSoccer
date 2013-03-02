//------------------------------------------------------------------------
// Name:    ProtocolDispatcher.h
// Author:  jjuiddong
// Date:    1/2/2013
// 
// ��Ʈ��ũ�κ��� ���� ��Ŷ�� �ش��ϴ� �������ݿ� �°� �м��ؼ� Listener�� 
// �Լ��� ȣ���Ѵ�.
// Dispatch �Լ����� ���� �������� �Լ����� ȣ���ϰ� �ȴ�.
// ��Ʈ��ũ �������� ���� �����Ϸ��� Dispatch()�� �ҽ����Ϸ� ������.
//------------------------------------------------------------------------
namespace network
{
	class IProtocolDispatcher
	{
	public:
		IProtocolDispatcher(int id) : m_Id(id) {}
		virtual ~IProtocolDispatcher() {}
		friend class CTaskLogic;
		friend class CClient;
		friend class CCoreClient;
	protected:
		int m_Id; // �����ϴ� protocol ID �� ������ ���̴�.
	public:
		int GetId() const { return m_Id; }
	protected:
		virtual void Dispatch(CPacket &packet, const ProtocolListenerList &listeners)=0;
	};
}
