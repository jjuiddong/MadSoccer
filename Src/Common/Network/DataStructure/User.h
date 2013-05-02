//------------------------------------------------------------------------
// Name:    User.h
// Author:  jjuiddong
// Date:    12/28/2012
// 
// 유저정보를 저장하는 클래스다.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	class CCharacter;
	class CUser
	{
	public:
		CUser();
		virtual ~CUser();

	protected:
		netid				m_NetId;
		int					m_Id;			// 유저 고유 아이디
		std::string		m_Name;

		typedef std::map<int,CCharacter*> CharacterMap;
		typedef CharacterMap::iterator CharacterItor;
		CharacterMap		m_CharacterMap;
		int					m_SelectCharacterId;

	public:
		void			SetNetId(netid netId) { m_NetId = netId; }
		void			SetId(int id) { m_Id = id; }
		void			SetName(const std::string &name) { m_Name = name; }
		netid		GetNetId() const { return m_NetId; }
		int			GetId() const { return m_Id; }
		const std::string& GetName() const { return m_Name; }

		bool			AddCharacter(CharacterPtr pChar);
		bool			RemoveCharacter(int characterId);
		CharacterPtr	GetCharacter(int characterId);
		CharacterPtr	GetSelectChracter();

	};
}
