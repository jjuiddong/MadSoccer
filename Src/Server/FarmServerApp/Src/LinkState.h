/**
Name:   LinkState.h
Author:  jjuiddong
Date:    4/8/2013

*/
#pragma once

class SLinkState
{
public:
	enum STATE
	{
		WAIT,
		CONNECT,
	};

	SLinkState() {}
	virtual ~SLinkState() {}

private:
	STATE m_State;
	network::SERVICE_TYPE m_Type;
	std::string m_Ip;
	int m_Port;

};
