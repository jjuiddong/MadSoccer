/**
Name:   Protocols.h
Author:  jjuiddong
Date:    4/17/2013

*/
#pragma once

namespace network { namespace protocols {

	void			Init();
	void			Cleanup();

	sProtocol* GetProtocol(int packetId);	

}}
