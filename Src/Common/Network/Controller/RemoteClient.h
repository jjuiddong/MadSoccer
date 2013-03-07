//------------------------------------------------------------------------
// Name:    RemoteClient.h
// Author:  jjuiddong
// Date:    12/25/2012
// 
// 서버에 접속된 클라이언트 정보를 가진다.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	DECLARE_TYPE_NAME_SCOPE(network, CRemoteClient)
	class CRemoteClient 
		: public sharedmemory::CSharedMem<CRemoteClient, TYPE_NAME(network::CRemoteClient)>
	{
	public:
		CRemoteClient();
		virtual ~CRemoteClient();

		netid		GetId() const;
		SOCKET	GetSocket() const;
		netid		GetGroupId() const;
		void			SetNetId(netid id) ;
		void			SetSocket(SOCKET sock);
		void			SetGroupId(netid groupid);

	protected:
		netid		m_Id;
		SOCKET	m_Socket;
		netid		m_GroupId;			//  join group
	};

	inline netid		CRemoteClient::GetId() const { return m_Id; }
	inline SOCKET	CRemoteClient::GetSocket() const { return m_Socket; }
	inline netid		CRemoteClient::GetGroupId() const { return m_GroupId; }
	inline void			CRemoteClient::SetNetId(netid id) { m_Id = id; }
	inline void			CRemoteClient::SetSocket(SOCKET sock) { m_Socket = sock; }
	inline void			CRemoteClient::SetGroupId(netid groupid) { m_GroupId = groupid; }

}
