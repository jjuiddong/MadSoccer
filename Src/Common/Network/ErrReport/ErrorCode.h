/**
Name:   ErrorCode.h
Author:  jjuiddong
Date:    3/15/2013


*/
#pragma once

namespace network { namespace error {

	enum ERROR_CODE
	{
		ERR_SUCCESS = 0,
		ERR_NO_ERROR = ERR_SUCCESS,

		ERR_P2PCONNECTION_NO_MEMBER_IN_GROUP,
		ERR_P2PCONNECTION_ALREADY_CONNECTED,
		ERR_P2PCONNECTION_HOSTCLIENT_DISAPPEAR,
		

	};

}}
