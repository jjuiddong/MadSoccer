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
	class CNetGroupDelegation : public CNetConnectorLinker
	{
	public:
		CNetGroupDelegation( const std::string svrType );
		virtual ~CNetGroupDelegation();

		void							SetConnector(NetGroupControllerPtr ptr);
		NetGroupControllerPtr		GetConnector() const;

	private:
		std::string				m_SvrType;
	};

}}
