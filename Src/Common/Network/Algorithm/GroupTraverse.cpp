
#include "stdafx.h"
#include "GroupTraverse.h"
#include "BindCore.h"
#include "../DataStructure/Group.h"
#include <boost/bind.hpp>

using namespace network;
using namespace group;


//------------------------------------------------------------------------
// boost::bind 에서 GroupPtr을 쓰면 인식하지 못해서 일단 CGroup*
// 를 인자로 받게했다.
//------------------------------------------------------------------------
template<class T>
CGroup* TraverseUpward( CGroup *pGroup, T &ty )
{
	RETV(!pGroup, NULL);
	if (ty(pGroup))
		return pGroup;
	return TraverseUpward(pGroup->GetParent(), ty);
}

template<class T>
CGroup* TraverseDownward( CGroup *pGroup, T &ty )
{
	RETV(!pGroup, NULL);
	if (ty(pGroup))
		return pGroup;

	BOOST_FOREACH(auto &child, pGroup->GetChildren())
	{
		CGroup *ptr = TraverseDownward(child, ty);
		if (ptr)
			return ptr;
	}
	return NULL;
}


//------------------------------------------------------------------------
// search up and down tree node
//------------------------------------------------------------------------
bool	group::IsP2PConnection( GroupPtr pGroup )
{
	RETV(!pGroup, false);
	GroupPtr ptr = GetP2PGroup(pGroup);
	return (ptr? true : false);
}


//------------------------------------------------------------------------
// search up and down tree node 
// to find p2p host client
//------------------------------------------------------------------------
netid group::GetP2PHostClient( GroupPtr pGroup )
{
	GroupPtr p2pGroup = GetP2PGroup(pGroup);
	if (!p2pGroup)
		return INVALID_NETID;

	const NetIdes &users = p2pGroup->GetUsers();
	BOOST_FOREACH(auto &user, users)
	{
		// ~~
	}

	return 0;
}


//------------------------------------------------------------------------
// return p2p group
// search up and down tree nod to find p2p group
// p2p group is only one node in tree line
//------------------------------------------------------------------------
GroupPtr group::GetP2PGroup( GroupPtr pGroup )
{
	RETV(!pGroup, NULL);
	if (pGroup->GetNetState() == CGroup::NET_STATE_P2P)
		return pGroup;

	GroupPtr ptr = TraverseUpward( pGroup, 
		boost::bind(&IsSameValue<CGroup::NET_STATE>,
			boost::bind(&CGroup::GetNetState, _1), CGroup::NET_STATE_P2P) );
	if (ptr)
		return ptr;

	ptr = TraverseDownward( pGroup, 
		boost::bind(&IsSameValue<CGroup::NET_STATE>,
			boost::bind(&CGroup::GetNetState, _1), CGroup::NET_STATE_P2P) );
	if (ptr)
		return ptr;

	return NULL;
}
