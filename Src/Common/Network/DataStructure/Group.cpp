
#include "stdafx.h"
#include "Group.h"

using namespace network;


CGroup::CGroup(GroupPtr parent, const std::string &name)  : 
	m_Id(common::GenerateId())
,	m_Name(name)
,	m_pParent(parent)
,	m_NetState(NET_STATE_SERVERCLIENT)
{
	m_Users.reserve(32);

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
