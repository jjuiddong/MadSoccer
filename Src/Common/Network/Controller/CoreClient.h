//------------------------------------------------------------------------
// Name:    ClientCore.h
// Author:  jjuiddong
// Date:    2/28/2013
// 
// CServer 에 접속하는 클라이언트 클래스다. 
//------------------------------------------------------------------------
#pragma once

#include "NetConnector.h"
#include "../interface/CoreClientEventListener.h"

namespace network
{
	class ICoreClientEventListener;
	class CCoreClient : public CNetConnector
	{
		friend class CNetLauncher;
		friend class CNetController;
		friend class CClient;
		typedef common::ReferencePtr<ICoreClientEventListener> CoreClientEventListenerPtr;

	public:
		CCoreClient(PROCESS_TYPE procType);
		virtual ~CCoreClient();

		PROCESS_TYPE GetProcessType() const;
		bool					IsConnect() const;
		void					SetEventListener(CoreClientEventListenerPtr ptr);
		void					SetThreadHandle(HANDLE handle);
		HANDLE			GetThreadHandle() const;

		bool					Stop();
		void					Disconnect();

		virtual bool		Send(netid netId, const CPacket &packet) override;
		virtual bool		SendAll(const CPacket &packet) override;

	protected:
		bool				Proc();
		void				Clear();
		void				SetConnect(bool isConnect) { m_IsConnect = isConnect; }
		void				SetServerIp(const std::string &ip) { m_ServerIP = ip; }
		void				SetServerPort(int port) { m_ServerPort = port; }

		// Event Handler
		void				OnConnect();
		void				OnDisconnect();

	protected:
		std::string							m_ServerIP;
		int										m_ServerPort;
		bool										m_IsConnect;
		CoreClientEventListenerPtr	m_pEventListener;
		PROCESS_TYPE					m_ServiceType;
		HANDLE								m_hThread;				// 소속된 스레드 핸들, 없다면 NULL

	};

	inline PROCESS_TYPE CCoreClient::GetProcessType() const { return m_ServiceType; }
	inline bool CCoreClient::IsConnect() const { return m_IsConnect; }
	inline void	 CCoreClient::SetEventListener(CoreClientEventListenerPtr ptr) { m_pEventListener = ptr; }
	inline void	 CCoreClient::SetThreadHandle(HANDLE handle) { m_hThread = handle; }
	inline HANDLE CCoreClient::GetThreadHandle() const { return m_hThread; }

}
