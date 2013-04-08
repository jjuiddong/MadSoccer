/**
Name:   MultiNetwork.h
Author:  jjuiddong
Date:    4/1/2013

 그룹으로 묶인 서버나 클라이언트들과 네트워킹하는 일을 담당한다.
 그룹에 접근하는 객체는 CNetGroupController 이고, 이 객체를 관리한다.
*/
#pragma once

#include "MultiNetworkUtillity.h"

namespace network { namespace multinetwork {

	class CNetGroupController;
	class CNetGroupDelegation;
	class CFarmServerConnector;

	DECLARE_TYPE_NAME_SCOPE(network::multinetwork, CMultiNetwork)
	class CMultiNetwork : public common::CSingleton<CMultiNetwork>
										,public memmonitor::Monitor<CMultiNetwork, TYPE_NAME(network::multinetwork::CMultiNetwork)>
	{
	public:
		CMultiNetwork();
		virtual ~CMultiNetwork();

		bool		Init( const std::string &configFileName );
		bool		Start();
		bool		Stop();
		bool		Proc();

		bool		AddController( CNetGroupController *ptr );
		bool		RemoveController( const std::string &linkSvrType );
		NetGroupControllerPtr GetController( const std::string &linkSvrType );
		bool		ConnectDelegation( const std::string &linkSvrType, NetGroupDelegationPtr ptr);

	protected:
		void		Cleanup();

	private:
		typedef common::VectorMap<std::string, NetGroupControllerPtr> Controllers;

		SSvrConfigData m_Config;
		Controllers m_Controllers;
		CFarmServerConnector *m_pFarmSvrConnector;

	};
}}
