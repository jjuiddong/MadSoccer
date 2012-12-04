
#pragma once


#include <list>
#include <algorithm>


namespace graphics
{
	class CDisplayObject;
	class CWindow;
	class CSyncNode;

	typedef std::list<CDisplayObject*> DispObjList;
	typedef DispObjList::iterator DispObjItor;

	typedef std::list<CWindow*> WindowList;
	typedef WindowList::iterator WindowItor;

	typedef std::list<CSyncNode*> SyncNodeList;
	typedef SyncNodeList::iterator SyncNodeItor;
	typedef SyncNodeList::const_iterator SyncNodeCItor;


	typedef std::list<Vector3> VertexList;
	typedef VertexList::iterator VtxItor;

	typedef std::list<Short2> IndexList;
	typedef IndexList::iterator IdxItor;


	enum STATE
	{
		INIT,			// 엔진 초기화 중
		RENDER,			// 엔진이 실행 중
		PAUSE,			// 렌더 일시 정지
		DISPLAYCHANGE,	// 디스플레이 설정 변경 중
		SHUTDOWN,		// 엔진 종료
	};

	enum TASK_ID
	{
		TASK_RENDERER = 1,		// 렌더러 (CRenderer)

	};

	enum TASK_MSG
	{
		// 렌더러 태스크
		MSG_RENDERER_SETSTATE,			// 상태 설정, wParam : state
		MSG_RENDERER_SETFPS,			// FPS 설정, wParam : fps
		MSG_RENDERER_SETROOTWINDOW,		// 루트윈도우 설정, wParam : rootWindow pointer
		MSG_RENDERER_RELEASE,			// 오브젝트 제거, wParam : type, lParam : pointer
		MSG_RENDERER_SETHWND,			// Hwnd 설정, wParam: hwnd

	};

}
