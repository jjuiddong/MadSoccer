//------------------------------------------------------------------------
// Name:    Group.h
// Author:  jjuiddong
// Date:    12/29/2012
// 
// �׷��� �����ϴ� �׷�, �������� ������ �����Ѵ�.
// �Ҹ��ڿ��� �ڽ����� ������ �׷���� ��� �����Ѵ�.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class CGroup
	{
	public:
		CGroup(GroupPtr parent=NULL, const std::string &name="");
		virtual ~CGroup();
		netid	GetNetId() const;
		const std::string& GetName() const;
		void SetName(const std::string &name);
		CGroup* GetParent() const;
		void SetParent(GroupPtr parent);

		// Group
		bool				AddChild( CGroup *pGroup );
		bool				RemoveChild( netid groupId );
		GroupPtr		GetChild(netid groupId );
		const GroupVector&	GetChildren() const { return m_Children.m_Seq; }

		// User
		bool				AddUser(netid groupId, netid userId);
		bool				RemoveUser(netid groupId, netid userId);
		bool				IsExistUser(netid groupId, netid userId);
		bool				IsExistUser(netid userId);
		const NetIdList&	GetUsers() const { return m_Users; }
		void				Clear();

		bool operator==(const CGroup &rhs) const { return m_NetId==rhs.GetNetId(); }

	protected:
		bool				AddUser(netid userId);
		bool				AddUserNApplyParent(GroupPtr pGroup, netid userId);
		bool				RemoveUser(netid userId);
		bool				RemoveUserNApplyParent(GroupPtr pGroup, netid userId);

	protected:
		netid			m_NetId;
		std::string	m_Name;
		NetIdList		m_Users;
		CGroup			*m_pParent;
		Groups			m_Children;
	};

	inline netid CGroup::GetNetId() const { return m_NetId; }
	inline const std::string& CGroup::GetName() const { return m_Name; }
	inline void CGroup::SetName(const std::string &name) { m_Name = name; }
	inline CGroup* CGroup::GetParent() const { return m_pParent; }
	inline void CGroup::SetParent(GroupPtr parent) { m_pParent = parent; }

}
