
#include "stdafx.h"
#include "User.h"
#include "Character.h"


using namespace network;


CUser::CUser() : 
	m_SelectCharacterId(-1)
{

}

CUser::~CUser() 
{

}


//------------------------------------------------------------------------
// ĳ���� �߰�
//------------------------------------------------------------------------
bool CUser::AddCharacter(CharacterPtr pChar)
{
	CharacterItor it = m_CharacterMap.find(pChar->GetId());
	if (m_CharacterMap.end() != it)
		return false;// �̹� �����ϸ� ����

	m_CharacterMap.insert( CharacterMap::value_type(pChar->GetId(), pChar) );
	return true;
}


//------------------------------------------------------------------------
// ĳ���� ����
//------------------------------------------------------------------------
bool CUser::RemoveCharacter(int characterId)
{
	CharacterItor it = m_CharacterMap.find(characterId);
	if (m_CharacterMap.end() == it)
		return false;
	delete it->second;
	m_CharacterMap.erase(it);
	return true;
}


//------------------------------------------------------------------------
// ĳ���� ���� ����
//------------------------------------------------------------------------
CharacterPtr CUser::GetCharacter(int characterId)
{
	CharacterItor it = m_CharacterMap.find(characterId);
	if (m_CharacterMap.end() == it)
		return NULL;
	return it->second;
}


//------------------------------------------------------------------------
// ���õ� ĳ���� ����
//------------------------------------------------------------------------
CharacterPtr CUser::GetSelectChracter()
{
	CharacterItor it = m_CharacterMap.find(m_SelectCharacterId);
	if (m_CharacterMap.end() == it)
		return NULL;
	return it->second;
}
