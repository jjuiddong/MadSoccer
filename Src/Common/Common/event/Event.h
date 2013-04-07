/**
Name:   Event.h
Author:  jjuiddong
Date:    4/4/2013

*/
#pragma once

namespace common
{

	enum EventType
	{
		EVT_NULL, // default

		// Server/Client
		EVT_CONNECT,
		EVT_DISCONNECT,

		// Server
		EVT_LISTEN,
		EVT_CLIENT_JOIN,
		EVT_CLIENT_LEAVE,

		// p2p
		EVT_P2P_CONNECT,
		EVT_P2P_DISCONNECT,
		EVT_MEMBER_JOIN,
		EVT_MEMBER_LEAVE,
	};


	/// Event Class
	class CEvent
	{
	public:
		CEvent();
		CEvent(EventType type);
		virtual ~CEvent() {}
		void			 SetType(EventType type);
		EventType GetType() const;
	private:
		EventType	m_Type;
	};

	inline CEvent::CEvent() { m_Type = EVT_NULL; }
	inline CEvent::CEvent(EventType type) : m_Type(type) { }
	inline void	CEvent::SetType(EventType type) { m_Type = type; }
	inline EventType CEvent::GetType() const { return m_Type; }

}
