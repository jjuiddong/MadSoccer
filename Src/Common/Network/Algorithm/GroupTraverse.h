//------------------------------------------------------------------------
// Name:    GroupTraverse.h
// Author:  jjuiddong
// Date:    2013-03-14
// 
// CGroup Tree Traverse algorithm functions
//------------------------------------------------------------------------
namespace network { namespace group {

	bool			IsP2PConnection( GroupPtr pGroup );

	netid		GetP2PHostClient( GroupPtr pGroup, IUserAccess &userAccess);

	GroupPtr	GetP2PGroup( GroupPtr pGroup );

	netid		SelectP2PHostClient( GroupPtr pGroup );
	
}}
