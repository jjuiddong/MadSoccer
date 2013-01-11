// ServerApp.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "ServerApp.h"

#include "NetCommon/login_Protocol.h"
#include "NetCommon/login_ProtocolListener.h"
#include "NetCommon/login_Protocol.cpp"
#include "NetCommon/login_ProtocolListener.cpp"

#include "NetCommon/basic_Protocol.h"
#include "NetCommon/basic_ProtocolListener.h"
#include "NetCommon/basic_Protocol.cpp"
#include "NetCommon/basic_ProtocolListener.cpp"


#include "Lib/ChatServer.h"
#include "Lib/ServerLauncher.h"
#include "Lib/LobbyServer.h"

CChatServer g_Server;
CLobbyServer g_LobbyServer;


// 전역 변수:
#define MAX_LOADSTRING 100
HINSTANCE hInst;								// 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.
HWND g_Hwnd = NULL;


// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	HACCEL hAccelTable;

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SERVERAPP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	CServerLauncher::Get()->Launcher( "script/serverstartconfig.txt" );

	network::Init(1);
	network::StartServer( 2333, &g_Server );
	network::StartServer( 2334, &g_LobbyServer );

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SERVERAPP));

	int tick = GetTickCount();
	bool bDoingBackgroundProcessing = true;
	while ( bDoingBackgroundProcessing ) 
	{ 
		//MSG msg;
		while ( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) 
		{ 
			if ( !GetMessage(&msg, NULL, 0, 0)) 
			{ 
				bDoingBackgroundProcessing = FALSE; 
				break; 
			}
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		const int curT = GetTickCount();
		if (curT - tick > 300)
		{
			InvalidateRect(g_Hwnd,NULL,TRUE);
			tick = curT;
		}
	}

	network::Clear();
	return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
//  설명:
//
//    Windows 95에서 추가된 'RegisterClassEx' 함수보다 먼저
//    해당 코드가 Win32 시스템과 호환되도록
//    하려는 경우에만 이 함수를 사용합니다. 이 함수를 호출해야
//    해당 응용 프로그램에 연결된
//    '올바른 형식의' 작은 아이콘을 가져올 수 있습니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SERVERAPP));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SERVERAPP);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 300, 300, NULL, NULL, hInstance, NULL);
   g_Hwnd = hWnd;

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


//------------------------------------------------------------------------
// 서버정보 출력
//------------------------------------------------------------------------
int PrintString(HDC hdc, int x, int y, const std::string &str)
{
	int fi = 0, li=0;
	while (1)
	{
		li = str.find('\n', fi);
		if (li < 0)
			break;
		std::string s0 = str.substr(fi, li-fi);
		TextOutA( hdc, x, y, s0.c_str(), s0.size() );
		y += 16;
		fi = li+1;
	}
	return y;
}
void Paint(HDC hdc)
{
	std::string str1 = g_LobbyServer.ToString();
	std::string str2 = network::ToString();
	int y = PrintString(hdc, 10, 0, str1);
	y += PrintString(hdc, 10, y+16, str2);
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
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Paint(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

