
#include "stdafx.h"
#include "Group.h"

using namespace network;

//------------------------------------------------------------------------
// 유저 추가
//------------------------------------------------------------------------
bool CGroup::AddUser(netid id)
{
	NetIdItor it = find(m_Users.begin(), m_Users.end(), id);
	if (m_Users.end() != it)
		return false; // 이미 존재한다면 실패
	m_Users.push_back( id );
	return true;
}


//------------------------------------------------------------------------
// 유저 제거
//------------------------------------------------------------------------
bool CGroup::RemoveUser(netid id)
{
	NetIdItor it = find(m_Users.begin(), m_Users.end(), id);
	if (m_Users.end() == it)
		return false; // 없다면 실패
	m_Users.erase(it);
	return true;
}


//------------------------------------------------------------------------
// User가 Group에 속해있다면 true를 리턴한다.
//------------------------------------------------------------------------
bool CGroup::IsExistUser(netid id)
{
	NetIdItor it = find(m_Users.begin(), m_Users.end(), id);
	return (m_Users.end() != it);
}


//------------------------------------------------------------------------
// 초기화
//------------------------------------------------------------------------
void CGroup::Clear()
{
	m_Users.clear();
}
