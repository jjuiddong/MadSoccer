//------------------------------------------------------------------------
// Name:    Group.h
// Author:  jjuiddong
// Date:    12/29/2012
// 
// �������� ������ �����Ѵ�.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class CGroup
	{
	public:
		CGroup() {}
		virtual ~CGroup() {}

	protected:
		int					m_Id;
		NetIdList			m_Users;

	public:
		int				GetId() const { return m_Id; }

		bool				AddUser(netid id);
		bool				RemoveUser(netid id);
		bool				IsExistUser(netid id);
		const NetIdList&	GetUsers() const { return m_Users; }
		void				Clear();
	};
}
