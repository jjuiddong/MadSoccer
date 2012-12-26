//------------------------------------------------------------------------
// Name:    Protocol.h
// Author:  jjuiddong
// Date:    12/23/2012
// 
// 서버와 클라이언트간의 주고받는 프로토콜을 정의한다.
// 네트워크 프로토콜 생성 컴파일러가 Dispatch()를 소스파일로 만들어낸다.
// 서버와 클라이언트간의 프로토콜은 *.prt 파일에 정의해야 한다.
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
