//------------------------------------------------------------------------
// Name:    VectorMap.h
// Author:  jjuiddong
// Date:    2/28/2013
// 
// Map �� Vector�� ���ĳ��� �ڷᱸ����.
// ���� �׼��� ���� Map�� �̿��ϰ�, ��ü�� Ž���� ����
// Vector�� �̿��Ѵ�.
// 
// VectorType �� Type�� ����ũ�� �����Ϳ��� �Ѵ�. ���� Type��
// �ִ� ��� Vector���� ����� �������� ���ϰ� �ȴ�.
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
				return false; // �̹� ���� ��
			m_RandomAccess.insert( vt );

			// insert vector
			m_Seq.push_back( vt.second );
			return true;
		}

		bool remove(const KeyType &key)
		{
			MapType::iterator it = m_RandomAccess.find(key);
			if (m_RandomAccess.end() == it)
				return false; // ���ٸ� ����

			removevector(it->second);
			m_RandomAccess.erase(it);
			return true;
		}

		// Type �� ����ũ�� ���� ����� �� �ִ� �Լ���.
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
			return false; // ��ã��
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
					if ((m_Seq.size()-1) > i) // elements�� ȸ���ؼ� �����Ѵ�.
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
