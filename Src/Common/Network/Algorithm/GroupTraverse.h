//------------------------------------------------------------------------
// Name:    GroupTraverse.h
// Author:  jjuiddong
// Date:    2013-03-14
// 
// CGroup Tree Traverse algorithm functions
//------------------------------------------------------------------------
namespace network { namespace group {

		bool		IsP2PConnection( GroupPtr pGroup );

		netid	GetP2PHostClient( GroupPtr pGroup );

		GroupPtr GetP2PGroup( GroupPtr pGroup );
	
}}
