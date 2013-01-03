//------------------------------------------------------------------------
// Name:    BindCore.h
// Author:  jjuiddong
// Date:    12/25/2012
// 
// STL �����̳ʿ� ���̴� Bind �Լ����� ��� ���Ҵ�.
//------------------------------------------------------------------------
#pragma once

namespace network
{
	template<class T>
	static bool IsSameSocket(T *p, SOCKET sock)
	{
		if (!p) return false;
		return p->GetSocket() == sock;
	} 

	template<class T>
	static bool IsSameNetId(T *p, netid netId)
	{
		if (!p) return false;
		return p->GetNetId() == netId;
	} 

	template<class T>
	static bool IsSameId(T *p, int id)
	{
		if (!p) return false;
		return p->GetId() == id;
	} 

	template<typename T>
	struct Match_Second 
	{
		T val;
		Match_Second ( T const & t ) : val ( t )
		{}
		template<typename Pair>
		bool operator() (Pair const & p) const 
		{
			return ( val == p.second );
		}
	};


}
