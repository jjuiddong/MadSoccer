/**
Name:   UserAccess.h
Author:  jjuiddong
Date:    3/17/2013

	interface class
	get the Remote Client in Server Object or Remote Client in Client Object
*/
#pragma once

namespace network
{
	class IUserAccess
	{
	public:
		virtual ~IUserAccess() {}
		virtual SessionPtr GetUser(netid id) = 0;
	};

}
