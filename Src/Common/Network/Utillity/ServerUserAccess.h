/**
Name:   ServerUserAccess.h
Author:  jjuiddong
Date:    3/17/2013

adapter class for server user map access 
*/
#pragma once

#include "UserAccess.h"
#include "../Service/ServerBasic.h"

namespace network
{
	class CServerUserAccess : public IUserAccess
	{
	public:
		CServerUserAccess( ServerBasicPtr svr );
		virtual ~CServerUserAccess();
		virtual RemoteClientPtr GetUser(netid id) override;
	protected:
		ServerBasicPtr m_pSvr;
	};


	inline CServerUserAccess::CServerUserAccess( ServerBasicPtr svr ) : 
		m_pSvr(svr)  
	{
	}

	inline CServerUserAccess::~CServerUserAccess() 
	{
	}

	/**
	 @brief return the remote client in server object
	 */
	inline RemoteClientPtr CServerUserAccess::GetUser(netid id)
	{
		RETV(!m_pSvr, NULL);
		return m_pSvr->GetRemoteClient(id);
	}

}
