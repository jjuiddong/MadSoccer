/**
Name:   NetGroupDelegation.h
Author:  jjuiddong
Date:    4/1/2013

network::multinetwork::CNetGroupDelegation ����
- Network Group �� �����ϴ� Controller�� ���� ��Ŷ�� ���޹ް�, �̺�Ʈ�� �ڵ鸵�Ѵ�.
*/
#pragma once

namespace network { namespace multinetwork {

	/// Network Group �� �����ϴ� Controller�� ���� ��Ŷ�� ���޹ް�, �̺�Ʈ�� �ڵ鸵�Ѵ�.
	class CMultiPlugDelegation : public CPlugLinker
	{
	public:
		CMultiPlugDelegation();
		virtual ~CMultiPlugDelegation();

		void		SetConnector(NetGroupControllerPtr ptr);
		NetGroupControllerPtr		GetConnector() const;

		CServerBasic* GetServer();
		const CoreClients_V& GetClients();
		CoreClientPtr GetClient(netid netId);
		CoreClientPtr GetClientFromServerNetId(netid serverNetId);

	protected:
		// Event 
		virtual void	OnConnectNetGroupController() { }

	private:
		std::string				m_SvrType;
	};

}}
