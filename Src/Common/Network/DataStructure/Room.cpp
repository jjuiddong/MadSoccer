
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
// ���� �߰�
//------------------------------------------------------------------------
bool CRoom::AddUser(int groupId, netid userId)
{
	GroupItor it = find_if(m_Groups.begin(), m_Groups.end(), bind(IsSameId<CGroup>,_1,groupId));
	if (m_Groups.end() == it)
		return false; // �ش��ϴ� Group�� ����
	return (*it)->AddUser(userId);
}


//------------------------------------------------------------------------
// ���� ����
//------------------------------------------------------------------------
bool CRoom::RemoveUser(int groupId, netid userId)
{
	GroupItor it = find_if(m_Groups.begin(), m_Groups.end(), bind(IsSameId<CGroup>,_1,groupId));
	if (m_Groups.end() == it)
		return false; // �ش��ϴ� Group�� ����
	return (*it)->RemoveUser(userId);
}


//------------------------------------------------------------------------
// �ʱ�ȭ
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
// �׷��߰�
// ���ڷ� �Ѿ�� Group Ŭ������ �״�� �����Ѵ�.
//------------------------------------------------------------------------
bool CRoom::AddGroup( CGroup *pGroup )
{
	GroupItor it = find_if(m_Groups.begin(), m_Groups.end(), 
		bind(IsSameId<CGroup>,_1,pGroup->GetId()));
	if (m_Groups.end() != it)
		return false; // �̹� �����Ѵٸ� ����
	m_Groups.push_back( pGroup );
	return true;
}


//------------------------------------------------------------------------
// �׷� ����
// �޸𸮵� ���ŵȴ�.
//------------------------------------------------------------------------
bool CRoom::RemoveGroup( int groupId )
{
	GroupItor it = find_if(m_Groups.begin(), m_Groups.end(), bind(IsSameId<CGroup>,_1,groupId));
	if (m_Groups.end() == it)
		return false; // ���ٸ� ����
	delete *it;
	m_Groups.erase(it);
	return true;
}


//------------------------------------------------------------------------
// �׷� ���
//------------------------------------------------------------------------
GroupPtr CRoom::GetGroup(int groupId )
{
	GroupItor it = find_if(m_Groups.begin(), m_Groups.end(), bind(IsSameId<CGroup>,_1,groupId));
	if (m_Groups.end() == it)
		return NULL; // ���ٸ� ����
	return *it;
}


//------------------------------------------------------------------------
// userId�� ���� ������ Group�� ���� �ִٸ� true�� �����Ѵ�.
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
