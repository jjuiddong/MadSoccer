//------------------------------------------------------------------------
// Name:    Group.h
// Author:  jjuiddong
// Date:    12/29/2012
// 
// 그룹을 관리하는 그룹, 여러명의 유저를 관리한다.
// 소멸자에서 자식으로 가지는 그룹들을 모두 제거한다.
//------------------------------------------------------------------------
#pragma once

namespace network { 
	class CPacket;
	class CGroup;
	namespace marshalling {
		CPacket& operator<<(CPacket& packet, const CGroup &rhs);
		CPacket& operator>>(CPacket& packet, CGroup &rhs);
}}


namespace network
{
	class CGroup
	{
		friend CPacket& (marshalling::operator<<(CPacket& packet, const CGroup &rhs));
		friend CPacket& (marshalling::operator>>(CPacket& packet, CGroup &rhs));

	public:
		CGroup(GroupPtr parent=NULL, const std::string &name="");
		virtual ~CGroup();

		// Group
		bool				AddChild( CGroup *pGroup );
		bool				RemoveChild( netid groupId );
		GroupPtr		GetChild( netid groupId );
		GroupPtr		GetChildFromUser( netid userId );
		const Groups::VectorType&	GetChildren() const { return m_Children.m_Seq; }

		// User
		bool				AddUser(netid groupId, netid userId);
		bool				RemoveUser(netid groupId, netid userId);
		bool				IsExistUser(netid groupId, netid userId);
		bool				IsExistUser(netid userId);
		const NetIdes&	GetUsers() const { return m_Users; }
		void				Clear();

		bool operator==(const CGroup &rhs) const { return m_Id==rhs.GetId(); }

		netid						GetId() const;
		const std::string&	GetName() const;
		void							SetName(const std::string &name);		
		DWORD					GetTag() const;
		void							SetTag(DWORD tag);
		CGroup*					GetParent() const;
		void							SetParent(GroupPtr parent);
		netid						GetParentId() const;

	protected:
		bool				AddUser(netid userId);
		bool				AddUserNApplyParent(GroupPtr pGroup, netid userId);
		bool				RemoveUser(netid userId);
		bool				RemoveUserNApplyParent(GroupPtr pGroup, netid userId);

	protected:
		netid			m_Id;
		netid			m_ParentId;
		std::string	m_Name;
		NetIdes		m_Users;
		DWORD		m_Tag;
		CGroup			*m_pParent;
		Groups			m_Children;
	};

	inline netid CGroup::GetId() const { return m_Id; }
	inline const std::string& CGroup::GetName() const { return m_Name; }
	inline void CGroup::SetName(const std::string &name) { m_Name = name; }
	inline CGroup* CGroup::GetParent() const { return m_pParent; }
	inline netid CGroup::GetParentId() const { return m_ParentId; }
	inline void CGroup::SetParent(GroupPtr parent) { m_pParent = parent; if(parent) {m_ParentId = parent->GetId();} }
	inline DWORD CGroup::GetTag() const { return m_Tag; }
	inline void CGroup::SetTag(DWORD tag) { m_Tag = tag; }

}
