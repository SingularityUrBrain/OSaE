// SP_LABS6.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Lab6.h"
#include <windows.h>
#include <tchar.h>
#include <wingdi.h>

#define IDM_FILE_QUIT 3
#define START_CLICKED 4
#define STOP_CLICKED 5
#define MAIN_WINDOW_WIDTH 800
#define MAIN_WINDOW_HEIGHT 400

int X = MAIN_WINDOW_WIDTH / 2 - 200;
int Y = MAIN_WINDOW_HEIGHT / 2 - 40;
bool backFlag = false;

void AddMenus(HWND hwnd)
{

	HMENU hMenubar;
	HMENU hMenu;

	hMenubar = CreateMenu();
	hMenu = CreateMenu();

	AppendMenuW(hMenu, MF_STRING, START_CLICKED, L"&Start");
	AppendMenuW(hMenu, MF_STRING, STOP_CLICKED, L"&Stop");
	AppendMenuW(hMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenuW(hMenu, MF_STRING, NULL, L"&Quit");

	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&Actions");
	SetMenu(hwnd, hMenubar);
}

LRESULT CALLBACK WndProc(
	HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	const int TIMER_ID = 1;
	switch (message)
	{
	case WM_CREATE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_FILE_QUIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case START_CLICKED:
		{
			UINT_PTR ret = SetTimer(hWnd, TIMER_ID, 50, NULL);
			if (ret == 0)
				MessageBox(hWnd, _T("Timer not set"), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
		}
		break;
		case STOP_CLICKED:
		{
			KillTimer(hWnd, TIMER_ID);
		}
		break;
		}

	case WM_TIMER:
	{
		if (X >= MAIN_WINDOW_WIDTH - 200)
		{
			backFlag = true;
		}
		if (X <= 150)
		{
			backFlag = false;
		}
		if (!backFlag)
		{
			X += 2;
		}
		else
		{
			X -= 2;
		}
		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;

	case WM_PAINT:
	{
		const char text[] = "lab6 - winapi";
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		TextOutA(hdc, X, Y, text, strlen(text));
		EndPaint(hWnd, &ps);
	}
	break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpzClassName)
{
	WNDCLASS wcWindowClass = { 0 };
	wcWindowClass.lpfnWndProc = (WNDPROC)WndProc;
	wcWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	wcWindowClass.hInstance = hInst;
	wcWindowClass.lpszClassName = lpzClassName;
	wcWindowClass.hCursor = (HICON)LoadImage(NULL, _T("./icon.ico"), IMAGE_CURSOR, 20, 20, LR_LOADFROMFILE);
	wcWindowClass.hIcon = (HICON)LoadImage(NULL, _T("./window.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wcWindowClass.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255, 255, 255)));
	return RegisterClass(&wcWindowClass);
}

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	LPCTSTR lpzClass = _T("My Window Class!");
	if (!RegMyWindowClass(hInstance, lpzClass))
		return 1;

	RECT screen_rect;
	GetWindowRect(GetDesktopWindow(), &screen_rect);
	int x = screen_rect.right / 2 - 150;
	int y = screen_rect.bottom / 2 - 75;

	HWND hWnd = CreateWindow(lpzClass, _T("Dialog Window"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, x, y, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, NULL, NULL,
		hInstance, NULL);

	if (!hWnd)
		return 2;

	AddMenus(hWnd);

	MSG msg = { 0 };
	int iGetOk = 0;
	while ((iGetOk = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (iGetOk == -1)
			return 3;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
