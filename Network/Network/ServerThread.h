//------------------------------------------------------------------------
// Name:    ServerThread.h
// Author:  jjuiddong
// Date:    2012-11-28
// 
// CServer���� ���̴� ��������� ��Ƴ��Ҵ�.
// 
// ������ �Ⱦ��� ���̴�.
//------------------------------------------------------------------------

#pragma once


namespace network
{
	void AcceptThread( void *pServerPtr );

	void RecvThread( void *pServerPtr );
	
	void WorkThread( void *pServerPtr );

}
