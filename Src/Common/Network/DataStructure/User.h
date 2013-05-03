/**
Name:   User.h
Author:  jjuiddong
Date:    12/28/2012

 유저정보를 저장하는 클래스다.
*/
#pragma once

namespace network
{
	class CCharacter;
	class CUser
	{
	public:
		CUser();
		virtual ~CUser();

		void			SetNetId(netid netId);
		void			SetCertifyKey(certify_key key);
		void			SetId(int id);
		void			SetName(const std::string &name);

		netid		GetNetId() const;
		certify_key GetCertifyKey() const;
		int			GetId() const;
		const std::string& GetName() const;

		bool			AddCharacter(CharacterPtr pChar);
		bool			RemoveCharacter(int characterId);
		CharacterPtr	GetCharacter(int characterId);
		CharacterPtr	GetSelectChracter();

	private:
		netid				m_NetId;
		certify_key		m_CertifyKey;
		int					m_Id;
		std::string		m_Name;

		typedef std::map<int,CCharacter*> CharacterMap;
		typedef CharacterMap::iterator CharacterItor;
		CharacterMap		m_CharacterMap;
		int					m_SelectCharacterId;

	};


	inline void	CUser::SetNetId(netid netId) { m_NetId = netId; }
	inline void	CUser::SetId(int id) { m_Id = id; }
	inline void	CUser::SetName(const std::string &name) { m_Name = name; }
	inline netid CUser::GetNetId() const { return m_NetId; }
	inline int CUser::GetId() const { return m_Id; }
	inline const std::string& CUser::GetName() const { return m_Name; }
	inline certify_key CUser::GetCertifyKey() const { return m_CertifyKey; }
	inline void			CUser::SetCertifyKey(certify_key key) { m_CertifyKey = key; }
}
