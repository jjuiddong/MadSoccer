
#include "stdafx.h"
#include "Room.h"
#include <boost/bind.hpp>

using namespace network; 


CRoom::CRoom()
{

}

CRoom::~CRoom()
{
	Clear();

}


//------------------------------------------------------------------------
// 유저 추가
//------------------------------------------------------------------------
bool CRoom::AddUser(int groupId, netid userId)
{
	GroupItor it = find_if(m_Groups.begin(), m_Groups.end(), bind(IsSameId<CGroup>,_1,groupId));
	if (m_Groups.end() == it)
		return false; // 해당하는 Group이 없음
	return (*it)->AddUser(userId);
}


//------------------------------------------------------------------------
// 유저 제거
//------------------------------------------------------------------------
bool CRoom::RemoveUser(int groupId, netid userId)
{
	GroupItor it = find_if(m_Groups.begin(), m_Groups.end(), bind(IsSameId<CGroup>,_1,groupId));
	if (m_Groups.end() == it)
		return false; // 해당하는 Group이 없음
	return (*it)->RemoveUser(userId);
}


//------------------------------------------------------------------------
// 초기화
//------------------------------------------------------------------------
void CRoom::Clear()
{
	BOOST_FOREACH(CGroup *p, m_Groups)
	{
		delete p;
	}
	m_Groups.clear();
}


//------------------------------------------------------------------------
// 그룹추가
// 인자로 넘어온 Group 클래스를 그대로 저장한다.
//------------------------------------------------------------------------
bool CRoom::AddGroup( CGroup *pGroup )
{
	GroupItor it = find_if(m_Groups.begin(), m_Groups.end(), 
		bind(IsSameId<CGroup>,_1,pGroup->GetId()));
	if (m_Groups.end() != it)
		return false; // 이미 존재한다면 실패
	m_Groups.push_back( pGroup );
	return true;
}


//------------------------------------------------------------------------
// 그룹 제거
// 메모리도 제거된다.
//------------------------------------------------------------------------
bool CRoom::RemoveGroup( int groupId )
{
	GroupItor it = find_if(m_Groups.begin(), m_Groups.end(), bind(IsSameId<CGroup>,_1,groupId));
	if (m_Groups.end() == it)
		return false; // 없다면 실패
	delete *it;
	m_Groups.erase(it);
	return true;
}


//------------------------------------------------------------------------
// 그룹 얻기
//------------------------------------------------------------------------
GroupPtr CRoom::GetGroup(int groupId )
{
	GroupItor it = find_if(m_Groups.begin(), m_Groups.end(), bind(IsSameId<CGroup>,_1,groupId));
	if (m_Groups.end() == it)
		return NULL; // 없다면 실패
	return *it;
}


//------------------------------------------------------------------------
// userId를 가진 유저가 Group에 속해 있다면 true를 리턴한다.
//------------------------------------------------------------------------
bool CRoom::IsUserExist(netid userId)
{
	BOOST_FOREACH(CGroup *p, m_Groups)
	{
		if (p->IsExistUser(userId))
			return true;
	}
	return false;
}
