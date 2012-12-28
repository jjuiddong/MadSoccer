//------------------------------------------------------------------------
// Name:    ProtocolListener.h
// Author:  jjuiddong
// Date:    12/26/2012
// 
// 프로토콜을 받아 처리하는 역할을 한다.
// Dispatch 함수에서 개별 프로토콜 함수들을 호출하게 된다.
// 네트워크 프로토콜 생성 컴파일러가 Dispatch()를 소스파일로 만들어낸다.
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
		int m_Id; // 대응하는 protocol ID 와 동일한 값이다.
	public:
		int GetId() const { return m_Id; }
		virtual void BeforePacketProcess(const CPacket &packet) {} // 패킷처리를 시작하기 전에 호출
		virtual void AfterPacketProcess(const CPacket &packet) {} // 패킷을 처리한 후 호출된다.
	protected:
		virtual void Dispatch(CPacket &packet, const ProtocolListenerList &listeners)=0;
	};
}
