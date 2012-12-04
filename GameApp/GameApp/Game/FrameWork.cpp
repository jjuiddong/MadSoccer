
#include "stdafx.h"
#include "FrameWork.h"
#include "../Resource.h"

using namespace game;

CFrameWork *CFrameWork::m_pInstance = NULL;


#define MAX_LOADSTRING 100
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//------------------------------------------------------------------------
// WindowMain 함수
//------------------------------------------------------------------------
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 전역 문자열을 초기화합니다.
	TCHAR titleName[ MAX_LOADSTRING];
	TCHAR className[ MAX_LOADSTRING];
	LoadString(hInstance, IDS_APP_TITLE, titleName, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GAMEAPP, className, MAX_LOADSTRING);
	CFrameWork::Get()->SetTitleName(titleName);
	CFrameWork::Get()->SetClassName(className);

	if (!CFrameWork::Get()->CreateWindowApp(hInstance, nCmdShow))
		return FALSE;

	CFrameWork::Get()->Init();
	CFrameWork::Get()->MainLoop();
	CFrameWork::Get()->Clear();

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 메뉴의 선택 영역을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			CFrameWork::Get()->ShutDown();
			break;
		case VK_RETURN:
			break;
		}

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



//------------------------------------------------------------------------
// 인스턴스 리턴
//------------------------------------------------------------------------
CFrameWork*	CFrameWork::Get()
{
	if (!m_pInstance)
		m_pInstance = CreateFrameWork();
	return m_pInstance;
}


CFrameWork::CFrameWork() :
	m_hWnd(NULL)
,	m_pRootWindow(NULL)
{

}

CFrameWork::~CFrameWork() 
{

}


//------------------------------------------------------------------------
// 클래스 초기화
//------------------------------------------------------------------------
void CFrameWork::Init()
{
	m_State = INIT;
	graphics::Init( m_hWnd );
	OnInitHandling();
}


//------------------------------------------------------------------------
// 매프래임마다 호출되는 함수
//------------------------------------------------------------------------
void CFrameWork::Proc()
{
	graphics::Proc();
	OnProcHandling();
}


//------------------------------------------------------------------------
// 프로그램 종료
//------------------------------------------------------------------------
void CFrameWork::ShutDown()
{
	m_State = SHUTDOWN;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CFrameWork::Clear()
{
	SAFE_RELEASE(m_pRootWindow);
	
	OnClearHandling();

	SAFE_DELETE(m_pInstance);
	graphics::ShutDown(); // 가장 나중에 호출되어야 한다.
}


//------------------------------------------------------------------------
// 윈도우 어플리케이션 생성
//------------------------------------------------------------------------
bool CFrameWork::CreateWindowApp(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMEAPP));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_GAMEAPP);
	wcex.lpszClassName	= m_ClassName;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassEx(&wcex);

	//	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
	m_hWnd = CreateWindow(m_ClassName, m_TitleName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 800, 600, NULL, NULL, hInstance, NULL);
	if (!m_hWnd)
	{
		return false;
	}

	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	m_hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMEAPP));

	return true;
}



//------------------------------------------------------------------------
// 매인 메세지 루프 
//------------------------------------------------------------------------
void CFrameWork::MainLoop()
{
	m_State = RUN; // FrameWork 실행

	MSG msg;
	while (RUN == m_State)
	{
		if (PeekMessage( &msg, m_hWnd, 0, 0, PM_NOREMOVE ))
		{
			GetMessage(&msg, NULL, 0, 0);
			if (!TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		Proc();
	}
}

