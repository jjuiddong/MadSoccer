
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
// 캐릭터 추가
//------------------------------------------------------------------------
bool CUser::AddCharacter(CharacterPtr pChar)
{
	CharacterItor it = m_CharacterMap.find(pChar->GetId());
	if (m_CharacterMap.end() != it)
		return false;// 이미 존재하면 실패

	m_CharacterMap.insert( CharacterMap::value_type(pChar->GetId(), pChar) );
	return true;
}


//------------------------------------------------------------------------
// 캐릭터 제거
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
// 캐릭터 정보 얻음
//------------------------------------------------------------------------
CharacterPtr CUser::GetCharacter(int characterId)
{
	CharacterItor it = m_CharacterMap.find(characterId);
	if (m_CharacterMap.end() == it)
		return NULL;
	return it->second;
}


//------------------------------------------------------------------------
// 선택된 캐릭터 리턴
//------------------------------------------------------------------------
CharacterPtr CUser::GetSelectChracter()
{
	CharacterItor it = m_CharacterMap.find(m_SelectCharacterId);
	if (m_CharacterMap.end() == it)
		return NULL;
	return it->second;
}
