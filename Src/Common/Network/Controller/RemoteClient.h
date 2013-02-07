//------------------------------------------------------------------------
// Name:    RemoteClient.h
// Author:  jjuiddong
// Date:    12/25/2012
// 
// 
//------------------------------------------------------------------------
#pragma once

namespace network
{
	DECLARE_TYPE_NAME_SCOPE(network, CRemoteClient)
	class CRemoteClient 
		: public sharedmemory::CSharedMem<CRemoteClient, TYPE_NAME(network::CRemoteClient)>
	{
	public:
		CRemoteClient() : m_Socket(0),	m_NetId(common::GenerateId()) { }
		virtual ~CRemoteClient() 
		{
			closesocket(m_Socket);
		}

	protected:
		netid		m_NetId;
		SOCKET		m_Socket;

	public:
		netid		GetNetId() const { return m_NetId; }
		SOCKET		GetSocket() const { return m_Socket; }
		void		SetNetId(netid id) { m_NetId = id; }
		void		SetSocket(SOCKET sock) { m_Socket = sock; }

	};
}
