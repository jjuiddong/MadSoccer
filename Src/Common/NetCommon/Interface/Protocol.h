/**
Name:   Protocol.h
Author:  jjuiddong
Date:    12/23/2012

 ������ Ŭ���̾�Ʈ���� �ְ�޴� ���������� �����Ѵ�.
 ������ Ŭ���̾�Ʈ���� ���������� *.prt ���Ͽ� �����ؾ� �Ѵ�.
*/
#pragma once

namespace network
{
	class IProtocol
	{
		friend class CNetConnector;

	public:
		IProtocol(int id) : m_Id(id) {}
		virtual ~IProtocol() {}
		void SetId(int id) { m_Id = id; }
		int GetId() const { return m_Id; }
		const NetConnectorPtr& GetNetConnector() const;
	protected:
		void SetNetConnector(NetConnectorPtr pServer);
	private:
		int m_Id; // �����ϴ� ProtocolListener ID �� ������ ���̴�.
		NetConnectorPtr m_pNetConnector;
	};


	inline const NetConnectorPtr& IProtocol::GetNetConnector() const { return m_pNetConnector; }
	inline void IProtocol::SetNetConnector(NetConnectorPtr pServer) { m_pNetConnector = pServer; }

}
