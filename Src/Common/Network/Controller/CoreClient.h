/**
Name:   CoreClient.h
Author:  jjuiddong
Date:    2/28/2013

CServer 에 접속하는 클라이언트 클래스다. 
*/
#pragma once

#include "../interface/CoreClientEventListener.h"

namespace network
{
	class ICoreClientEventListener;
	class CCoreClient : public CNetConnector
	{
		friend class CNetLauncher;
		friend class CNetController;
		friend class CClientBasic;
		friend class CP2PClient;
		typedef common::ReferencePtr<ICoreClientEventListener> CoreClientEventListenerPtr;

	public:
		CCoreClient(PROCESS_TYPE procType);
		virtual ~CCoreClient();

		bool					Stop();
		void					Disconnect();
		void					Close();

		bool					IsConnect() const;
		void					SetEventListener(CoreClientEventListenerPtr ptr);
		void					SetConnect(bool isConnect);
		void					SetServerIp(const std::string &ip);
		void					SetServerPort(int port);

		virtual bool		Send(netid netId, const SEND_FLAG flag, const CPacket &packet);
		virtual bool		SendAll(const CPacket &packet) override;

		// Event Handler
		void				OnConnect();
		void				OnDisconnect();

	protected:
		bool				Proc();
		void				DispatchPacket();
		void				Clear();

	protected:
		std::string							m_ServerIP;
		int										m_ServerPort;
		bool										m_IsConnect;
		CoreClientEventListenerPtr	m_pEventListener;

	};

	inline bool CCoreClient::IsConnect() const { return m_IsConnect; }
	inline void	 CCoreClient::SetEventListener(CoreClientEventListenerPtr ptr) { m_pEventListener = ptr; }
	inline void	 CCoreClient::SetConnect(bool isConnect) { m_IsConnect = isConnect; }
	inline void	 CCoreClient::SetServerIp(const std::string &ip) { m_ServerIP = ip; }
	inline void	 CCoreClient::SetServerPort(int port) { m_ServerPort = port; }


}
