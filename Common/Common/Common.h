
#ifndef __COMMON_H__
#define __COMMON_H__


#include <Windows.h>
#include <list>
#include <map>
#include <algorithm>
#include <string>


namespace common
{
	class CTask;
	typedef std::list<CTask*> TaskList;
	typedef TaskList::iterator TaskItor;


	// ������� �������� �ְ� �޴� �޼��� ����
	typedef struct _SExternalMsg
	{
		int rcvTaskId;			// �޼����� ���� Task ID (0=tread, -1=�ܺο��� �޴� �޼���)
		int msg;
		WPARAM wParam;
		LPARAM lParam;

		_SExternalMsg() {}
		_SExternalMsg( int rcvtaskId, int msgtype, WPARAM wparam, LPARAM lparam):
			rcvTaskId(rcvtaskId), msg(msgtype), wParam(wparam), lParam(lparam)
			{
			}

	} SExternalMsg;

	typedef std::list<SExternalMsg> ExternalMsgList;
	typedef ExternalMsgList::iterator ExternalMsgItor;



}



#include "math/Math.h"
#include "thread/Thread.h"
#include "thread/Task.h"


#define SAFE_DELETE(p) {if (p) { delete p; p=NULL;}}
#define SAFE_DELETEA(p) {if (p) { delete[] p; p=NULL;}}

// WinDef.h �� ���� �Ǿ� �ִ�.
//#define IN		// �Լ� ������ �Է����� ���� �Ķ���� (������)
//#define OUT		// �Լ� ���ڷ� ���� ���� �����Ǿ� ���ϵǴ� �Ķ����




#endif // __COMMON_H__
