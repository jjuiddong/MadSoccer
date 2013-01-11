// ServerApp.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
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


// ���� ����:
#define MAX_LOADSTRING 100
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.
HWND g_Hwnd = NULL;


// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
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

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SERVERAPP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
//  ����:
//
//    Windows 95���� �߰��� 'RegisterClassEx' �Լ����� ����
//    �ش� �ڵ尡 Win32 �ý��۰� ȣȯ�ǵ���
//    �Ϸ��� ��쿡�� �� �Լ��� ����մϴ�. �� �Լ��� ȣ���ؾ�
//    �ش� ���� ���α׷��� �����
//    '�ùٸ� ������' ���� �������� ������ �� �ֽ��ϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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
// �������� ���
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
		// �޴��� ���� ������ ���� �м��մϴ�.
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

