// ServerApp.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "ServerApp.h"

#include "NetProtocol/Src/login_Protocol.h"
#include "NetProtocol/Src/login_ProtocolListener.h"
#include "NetProtocol/Src/login_Protocol.cpp"
#include "NetProtocol/Src/login_ProtocolListener.cpp"

#include "NetProtocol/Src/basic_Protocol.h"
#include "NetProtocol/Src/basic_ProtocolListener.h"

#include "Lib/ChatServer.h"
#include "Lib/ServerLauncher.h"
#include "Lib/LobbyServer.h"

#include "wxMemMonitorLib/wxMemMonitor.h"
MEMORYMONITOR_INNER_PROCESS();

#pragma comment(lib, "../../../Lib/Vld/Win32/vld.lib")
#include "../../../Lib/Vld/vld.h"


//CChatServer *g_pChatServer;
CLobbyServer *g_pLobbyServer;

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
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SERVERAPP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	if (!memmonitor::Init( memmonitor::INNER_PROCESS, hInstance, "madsoccer_server_monitor.json" ))
	{
		clog::Error(log::ERROR_CRITICAL, "memmonitor::init() fail !!\n" );
		goto exit;
	}
	if (!network::Init(1, "madsoccer_server_config.json"))
	{
		clog::Error(log::ERROR_CRITICAL, "network::init() fail !!\n" );
		goto exit;
	}
	g_pLobbyServer = new CLobbyServer();
	if (!network::ConnectDelegation( "client", g_pLobbyServer ))
	{
		clog::Error(log::ERROR_CRITICAL, "network::ConnectDelegation() fail !!\n" );
		goto exit;
	}
	
	network::StartMultiNetwork();

	MSG msg;
	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SERVERAPP));
	int tick = GetTickCount();
	bool bDoingBackgroundProcessing = true;
	while ( bDoingBackgroundProcessing ) 
	{ 
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

exit:
	network::Clear();
	SAFE_DELETE(g_pLobbyServer);
	//SAFE_DELETE(g_pChatServer);
	memmonitor::Cleanup();

	return 0;
}


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

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      300, 0, 300, 300, NULL, NULL, hInstance, NULL);
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
	if (g_pLobbyServer)
	{
		std::string str1 = g_pLobbyServer->ToString();
		std::string str2 = network::ToString();
		int y = PrintString(hdc, 10, 0, str1);
		y += PrintString(hdc, 10, y+16, str2);
	}
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

