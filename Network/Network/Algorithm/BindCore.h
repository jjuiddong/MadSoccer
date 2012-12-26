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


}
