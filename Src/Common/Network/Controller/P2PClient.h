//------------------------------------------------------------------------
// Name:    P2PClient.h
// Author:  jjuiddong
// Date:    2/28/2013
// 
// P2P 통신을 하는 클래스다.
//------------------------------------------------------------------------
#pragma once

#include "NetConnector.h"

namespace network
{
	class CServer;
	class CCoreClient;
	class CP2PClient : public CNetConnector
	{
	public:
		CP2PClient();
		virtual ~CP2PClient();

	protected:
		CCoreClient	*m_pP2pClient;
		CServer		*m_pP2pHost;

	public:
		bool				Stop();
		bool				Proc();
		void				Clear();

		bool				IsConnect() const { return true; }

		// child implementes
		virtual bool		Send(netid netId, const CPacket &packet);
		virtual bool		SendAll(const CPacket &packet);

	};
}
