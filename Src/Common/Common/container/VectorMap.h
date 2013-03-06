//------------------------------------------------------------------------
// Name:    VectorMap.h
// Author:  jjuiddong
// Date:    2/28/2013
// 
// Map 과 Vector를 합쳐놓은 자료구조다.
// 랜덤 액세스 때는 Map을 이용하고, 전체를 탐색할 때는
// Vector를 이용한다.
// 
// VectorType 의 Type은 유니크한 데이터여야 한다. 같은 Type이
// 있는 경우 Vector에서 제대로 제거하지 못하게 된다.
//------------------------------------------------------------------------
#pragma once

namespace common
{
	template <class KeyType, class Type>
	class VectorMap
	{
	public:
		typedef std::map<KeyType, Type> MapType;
		typedef std::vector<Type> VectorType;
		typedef typename MapType::iterator iterator;
		typedef typename MapType::value_type value_type;

		VectorMap() {}
		VectorMap(int reserved) : m_Seq(reserved) {}

		bool insert(const value_type &vt)
		{
			// insert map
			MapType::iterator it = m_RandomAccess.find(vt.first);
			if (m_RandomAccess.end() != it)
				return false; // 이미 존재 함
			m_RandomAccess.insert( vt );

			// insert vector
			m_Seq.push_back( vt.second );
			return true;
		}

		bool remove(const KeyType &key)
		{
			MapType::iterator it = m_RandomAccess.find(key);
			if (m_RandomAccess.end() == it)
				return false; // 없다면 리턴

			removevector(it->second);
			m_RandomAccess.erase(it);
			return true;
		}

		// Type 이 유니크할 때만 사용할 수 있는 함수다.
		bool removebytype(const Type &ty)
		{
			MapType::iterator it = m_RandomAccess.begin();
			while (m_RandomAccess.end() != it)
			{
				if (ty == it->second)
				{
					removevector(it->second);
					m_RandomAccess.erase(it);
					return true;
				}
			}
			return false; // 못찾음
		}

		iterator find(const KeyType &key) { return m_RandomAccess.find(key); }
		iterator begin() { return m_RandomAccess.begin(); }
		iterator end() { return m_RandomAccess.end(); }

	protected:
		void removevector(const Type &ty)
		{
			for (u_int i=0; i < m_Seq.size(); ++i)
			{
				if (m_Seq[ i] == ty)
				{
					if ((m_Seq.size()-1) > i) // elements를 회전해서 제거한다.
						std::rotate(m_Seq.begin()+i, m_Seq.begin()+i+1, m_Seq.end() );
					m_Seq.pop_back();
					break;
				}
			}
		}

	public:
		std::map<KeyType, Type>	m_RandomAccess;
		std::vector<Type>				m_Seq;

	};

}
