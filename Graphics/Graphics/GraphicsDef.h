
#pragma once


#include <list>
#include <algorithm>


namespace graphics
{
	class CDisplayObject;
	class CWindow;

	typedef std::list<CDisplayObject*> DispObjList;
	typedef DispObjList::iterator DispObjItor;

	typedef std::list<CWindow*> WindowList;
	typedef WindowList::iterator WindowItor;


	enum STATE
	{
		INIT,			// ���� �ʱ�ȭ ��
		RENDER,			// ������ ���� ��
		PAUSE,			// ���� �Ͻ� ����
		DISPLAYCHANGE,	// ���÷��� ���� ���� ��
		SHUTDOWN,		// ���� ����
	};

	enum TASK_ID
	{
		TASK_RENDERER = 1,		// ������ (CRenderer)

	};

	enum TASK_MSG
	{
		// ������ �½�ũ
		MSG_RENDERER_SETSTATE,			// ���� ����, wParam : state
		MSG_RENDERER_SETFPS,			// FPS ����, wParam : fps
		MSG_RENDERER_SETROOTWINDOW,		// ��Ʈ������ ����, wParam : rootWindow pointer
		MSG_RENDERER_RELEASE,			// ������Ʈ ����, wParam : type, lParam : pointer
		MSG_RENDERER_SETHWND,			// Hwnd ����, wParam: hwnd

	};

}
