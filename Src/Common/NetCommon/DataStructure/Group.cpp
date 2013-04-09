
#include "stdafx.h"
#include "Group.h"
#include "../Interface/Factory.h"

using namespace network;


CGroup::CGroup(GroupPtr parent, const std::string &name)  : 
	m_Id(common::GenerateId())
,	m_Name(name)
,	m_pParent(parent)
,	m_NetState(NET_STATE_SERVERCLIENT)
{
	//m_Users.reserve(32);

}

// copy constructor
CGroup::CGroup(const CGroup &rhs)
{
	this->operator=(rhs);
}


CGroup::~CGroup() 
{
	Clear();
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool	CGroup::AddChild( CGroup *pGroup )
{
	RETV(!pGroup,false);

	GroupPtr ptr = GetChild(pGroup->GetId());
	if (ptr) return false; // already exist

	pGroup->SetParent(this);
	m_Children.insert(Groups::value_type(pGroup->GetId(), pGroup));
	
	// add group member
	BOOST_FOREACH(auto userid, pGroup->GetUsers())
	{
		AddUserNApplyParent(this, userid);
	}
	return true;
}


/**
 @brief 비어있는 Group을 생성한다.

 이미 group에 유저가 있으면 group이 자식으로 group을 생성할 수 없다.
 만약 이렇게 하려면, group에 소속된 멤버들을 새 그룹에 소속시키고, 
 현재 group의 자식으로 추가해야 한다. 

 즉, 간단히 말하면,
 단말 노드에는 그룹을 추가할 수 없다.
 단말 노드에만 유저가 소속될 수 있다. 
 */
CGroup* CGroup::AddChild( IGroupFactory *pFactory )
{
	RETV(!pFactory, NULL);
	if (IsTerminal())
		return NULL;
	CGroup *pNewGroup = pFactory->New();
	if (!AddChild(pNewGroup))
	{
		delete pNewGroup;
		return NULL;
	}
	return pNewGroup;
}


//------------------------------------------------------------------------
// delete CGroup Object memory
//------------------------------------------------------------------------
bool	CGroup::RemoveChild( netid groupId )
{
	GroupPtr pGroup = GetChild(groupId);
	if (!pGroup) return false; // not exist

	// remove group member
	BOOST_FOREACH(auto userid, m_Users)
	{
		RemoveUserNApplyParent(pGroup->GetParent(), userid);
	}

	GroupPtr parent = pGroup->GetParent();
	if (parent)
	{
		auto it = parent->m_Children.find(groupId);
		if (parent->m_Children.end() == it)
			return false; // not exist member

		SAFE_DELETE(it->second);
		parent->m_Children.remove(groupId);	
	}
	return true;
}


//------------------------------------------------------------------------
// return child group object
//------------------------------------------------------------------------
GroupPtr CGroup::GetChild(netid groupId )
{
	auto it = m_Children.find( groupId );
	if (m_Children.end() != it)
		return GroupPtr(it->second);

	// search children
	BOOST_FOREACH(auto &child, m_Children.m_Seq)
	{
		GroupPtr ptr = child->GetChild(groupId);
		if (ptr) return ptr;
	}
	return NULL;
}


/**
 @brief return child or current of object same id
 */
GroupPtr	CGroup::GetChildandThis( netid groupId )
{
	if (GetId() == groupId)
		return this;
	return GetChild(groupId);
}


//------------------------------------------------------------------------
// return leaf group node object
//------------------------------------------------------------------------
GroupPtr CGroup::GetChildFromUser( netid userId )
{
	if (!IsExistUser(userId))
		return NULL;

	if (m_Children.empty()) // leaf node
		return this;

	// search children
	BOOST_FOREACH(auto &child, m_Children.m_Seq)
	{		
		GroupPtr ptr = child->GetChildFromUser( userId );
		if (ptr) return ptr;
	}

	return this;
}


/**
 @brief if this group is terminal node, return true or return false
 */
bool	CGroup::IsTerminal()
{
	if ((m_Children.size() <= 0) && (m_Users.size() > 0))
		return true;
	return false;
}


//------------------------------------------------------------------------
// 유저 추가
//------------------------------------------------------------------------
bool CGroup::AddUser(netid groupId, netid userId)
{
	GroupPtr pGroup = (GetId() == groupId)? this : GetChild(groupId);
	if(!pGroup) return false; // not exist group
	return AddUserNApplyParent(pGroup, userId);	
}


//------------------------------------------------------------------------
// 유저 제거
//------------------------------------------------------------------------
bool CGroup::RemoveUser(netid groupId, netid userId)
{
	GroupPtr pGroup = (GetId() == groupId)? this : GetChild(groupId);
	if(!pGroup) return false; // not exist group
	return RemoveUserNApplyParent(pGroup, userId);
}


//------------------------------------------------------------------------
// User가 Group에 속해있다면 true를 리턴한다.
//------------------------------------------------------------------------
bool CGroup::IsExistUser(netid groupId, netid userId)
{
	GroupPtr pGroup = GetChild(groupId);
	if(!pGroup) return false; // not exist group

	auto it = find(m_Users.begin(), m_Users.end(), userId);
	return (m_Users.end() != it);
}


//------------------------------------------------------------------------
// 유저 추가
//------------------------------------------------------------------------
bool CGroup::AddUser(netid userId)
{
	auto it = find(m_Users.begin(), m_Users.end(), userId);
	if (m_Users.end() != it)
		return false; // 이미 존재한다면 실패
	m_Users.push_back( userId );
	return true;
}


//------------------------------------------------------------------------
// Add UserId to Group Object 
// and Make Group Tree
//------------------------------------------------------------------------
bool	CGroup::AddUserNApplyParent(GroupPtr pGroup, netid userId)
{
	if (!pGroup) 
		return true;
	if (!pGroup->AddUser(userId))
		return false;
	return AddUserNApplyParent(pGroup->GetParent(), userId);
}


//------------------------------------------------------------------------
// 유저 제거
//------------------------------------------------------------------------
bool CGroup::RemoveUser(netid userId)
{
	//auto it = find(m_Users.begin(), m_Users.end(), userId);
	//if (m_Users.end() == it)
	//	return false; // 없다면 실패
	//m_Users.erase(it);
	return common::removevector(m_Users, userId);
}


//------------------------------------------------------------------------
// Remove userId from Group Object
// and Apply Parent Group Node
//------------------------------------------------------------------------
bool	CGroup::RemoveUserNApplyParent(GroupPtr pGroup, netid userId)
{
	if (!pGroup) 
		return true;
	if (!pGroup->RemoveUser(userId))
		return false;
	return RemoveUserNApplyParent(pGroup->GetParent(), userId);
}


/**
 @brief Add Viewer
 */
bool	CGroup::AddViewer(netid id)
{
	auto it = find(m_Viewers.begin(), m_Viewers.end(), id);
	if (m_Viewers.end() != it)
		return false; // already exist, return
	m_Viewers.push_back( id );
	return true;
}


/**
 @brief Remove Viewer
 */
bool	CGroup::RemoveViewer(netid id)
{
	return common::removevector(m_Viewers, id);
}


//------------------------------------------------------------------------
// User가 Group에 속해있다면 true를 리턴한다.
//------------------------------------------------------------------------
bool CGroup::IsExistUser(netid userId)
{
	auto it = find(m_Users.begin(), m_Users.end(), userId);
	return (m_Users.end() != it);
}


//------------------------------------------------------------------------
// 초기화
//------------------------------------------------------------------------
void CGroup::Clear()
{
	BOOST_FOREACH(auto &pGroup, m_Children.m_Seq)
	{
		SAFE_DELETE(pGroup);
	}
	m_Children.clear();
	m_Users.clear();
}


/**
 @brief 
 */
CGroup& CGroup::operator=(const CGroup &rhs)
{
	if (this != &rhs)
	{
		m_Id = rhs.m_Id;
		m_ParentId = rhs.m_ParentId;
		m_Name = rhs.m_Name;
		m_Tag = rhs.m_Tag;
		m_NetState = rhs.m_NetState;
		m_pParent = NULL;
		m_Users = rhs.m_Users;
		m_Viewers = rhs.m_Viewers;
	}
	return *this;
}
