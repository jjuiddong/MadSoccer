//------------------------------------------------------------------------
// Name:    Protocol.h
// Author:  jjuiddong
// Date:    12/23/2012
// 
// 서버와 클라이언트간의 주고받는 프로토콜을 정의한다.
// 서버와 클라이언트간의 프로토콜은 *.prt 파일에 정의해야 한다.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class IProtocol
	{
	protected:
		IProtocol(int id) : m_Id(id) {}
		virtual ~IProtocol() {}
		int m_Id; // 대응하는 ProtocolListener ID 와 동일한 값이다.
		NetConnectorPtr m_pNetConnector;
	public:
		void SetId(int id) { m_Id = id; }
		int GetId() const { return m_Id; }
		void SetNetConnector(NetConnectorPtr pServer) { m_pNetConnector = pServer; }
		const NetConnectorPtr& GetNetConnector() const { return m_pNetConnector; }
	};
}
