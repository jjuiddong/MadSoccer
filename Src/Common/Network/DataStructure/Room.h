//------------------------------------------------------------------------
// Name:    Room.h
// Author:  jjuiddong
// Date:    12/21/2012
// 
// 방정보를 정의한다.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class CRoom
	{
	public:
		CRoom();
		virtual ~CRoom();

	protected:
		int					m_Id;
		int					m_Number;	// 방번호
		std::string		m_Name;
		GroupList			m_Groups;

	public:
		void				SetNumber(int number) { m_Number = number; }
		void				SetName(const std::string &name ) { m_Name = name; }
		void				SetId(int id) { m_Id = id; }
		int					GetId() const { return m_Id; }
		const std::string&	GetName() const { return m_Name; }

		bool				AddGroup( CGroup *pGroup );
		bool				RemoveGroup( int groupId );
		const GroupList&	GetGroups() const { return m_Groups; }
		GroupPtr			GetGroup(int groupId );

		bool				AddUser(int groupId, netid userId);
		bool				RemoveUser(int groupId, netid userId);
		bool				IsUserExist(netid userId);
		void				Clear();

	};
}
