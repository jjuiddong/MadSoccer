//------------------------------------------------------------------------
// Name:    ServerThread.h
// Author:  jjuiddong
// Date:    2012-11-28
// 
// CServer에서 쓰이는 쓰레드들을 모아놓았다.
// 
// 지금은 안쓰는 중이다.
//------------------------------------------------------------------------

#pragma once


namespace network
{
	void AcceptThread( void *pServerPtr );

	void RecvThread( void *pServerPtr );
	
	void WorkThread( void *pServerPtr );

}
