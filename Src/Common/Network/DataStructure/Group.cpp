
#include "stdafx.h"
#include "Group.h"

using namespace network;

//------------------------------------------------------------------------
// ���� �߰�
//------------------------------------------------------------------------
bool CGroup::AddUser(netid id)
{
	NetIdItor it = find(m_Users.begin(), m_Users.end(), id);
	if (m_Users.end() != it)
		return false; // �̹� �����Ѵٸ� ����
	m_Users.push_back( id );
	return true;
}


//------------------------------------------------------------------------
// ���� ����
//------------------------------------------------------------------------
bool CGroup::RemoveUser(netid id)
{
	NetIdItor it = find(m_Users.begin(), m_Users.end(), id);
	if (m_Users.end() == it)
		return false; // ���ٸ� ����
	m_Users.erase(it);
	return true;
}


//------------------------------------------------------------------------
// User�� Group�� �����ִٸ� true�� �����Ѵ�.
//------------------------------------------------------------------------
bool CGroup::IsExistUser(netid id)
{
	NetIdItor it = find(m_Users.begin(), m_Users.end(), id);
	return (m_Users.end() != it);
}


//------------------------------------------------------------------------
// �ʱ�ȭ
//------------------------------------------------------------------------
void CGroup::Clear()
{
	m_Users.clear();
}
