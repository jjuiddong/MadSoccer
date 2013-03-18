//------------------------------------------------------------------------
// Name:    Protocol.h
// Author:  jjuiddong
// Date:    12/23/2012
// 
// ������ Ŭ���̾�Ʈ���� �ְ�޴� ���������� �����Ѵ�.
// ������ Ŭ���̾�Ʈ���� ���������� *.prt ���Ͽ� �����ؾ� �Ѵ�.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class IProtocol
	{
	protected:
		IProtocol(int id) : m_Id(id) {}
		virtual ~IProtocol() {}
		int m_Id; // �����ϴ� ProtocolListener ID �� ������ ���̴�.
		NetConnectorPtr m_pNetConnector;
	public:
		void SetId(int id) { m_Id = id; }
		int GetId() const { return m_Id; }
		void SetNetConnector(NetConnectorPtr pServer) { m_pNetConnector = pServer; }
		const NetConnectorPtr& GetNetConnector() const { return m_pNetConnector; }
	};
}
