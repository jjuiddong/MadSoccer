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
		friend class CCoreClient;
		friend class CServerBasic;

		int GetId() const;

	protected:
		virtual void Dispatch(CPacket &packet, const ProtocolListenerList &listeners)=0;
		int m_Id; // �����ϴ� protocol ID �� ������ ���̴�.
	};

	inline int IProtocolDispatcher::GetId() const { return m_Id; }

}
