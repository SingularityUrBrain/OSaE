// WindowsProject5x1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab10_1.h"

#define MAX_LOADSTRING 100

#define DRAW_MESSAGE L"DrawMessage"
#define COLOR_MESSAGE L"ColorMessage"
#define SHAPE_MESSAGE L"ShapeMessage"

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND main_hwnd;
UINT WM_DRAW;
UINT WM_COLOR;
UINT WM_SHAPE;
bool draw = false;
int color = 0, shape = 0;
int drawX = -1, drawY = -1;

void debug(int lol)
{
    char b[15];
    _itoa_s((int)lol, b, 15, 10);
    SetWindowTextA(main_hwnd, b);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB101, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB101));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB101));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB101);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    WM_DRAW = RegisterWindowMessage((LPCWSTR)DRAW_MESSAGE);
    WM_COLOR = RegisterWindowMessage((LPCWSTR)COLOR_MESSAGE);
    WM_SHAPE = RegisterWindowMessage((LPCWSTR)SHAPE_MESSAGE);

    main_hwnd = hWnd;

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;

    if (message == WM_DRAW)
    {
        //debug(111);
        draw = lParam;
        //InvalidateRect(hWnd, NULL, true);
    }
    else if (message == WM_COLOR)
    {
        //debug(222);
        color = lParam;
        //InvalidateRect(hWnd, NULL, true);
    }
    else if (message == WM_SHAPE)
    {
        //debug(333);
        shape = lParam;
        //InvalidateRect(hWnd, NULL, true);
    }
    else switch (message)
    {/* так не работает
    case WM_DRAW:
        draw = lParam;
        break;
    case WM_COLOR:
        color = lParam;
        InvalidateRect(hWnd, NULL, true);
    case WM_SHAPE:
        shape = lParam;
        InvalidateRect(hWnd, NULL, true);*/
    case WM_LBUTTONDOWN:
        //debug((int)draw);
        drawX = LOWORD(lParam);
        drawY = HIWORD(lParam);
        InvalidateRect(hWnd, NULL, true);
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        if (draw && drawX != -1) {
            HBRUSH brush = NULL;
            switch (color)
            {
            case 0:
                brush = CreateSolidBrush(RGB(255, 0, 0));
                break;
            case 1:
                brush = CreateSolidBrush(RGB(0, 255, 0));
                break;
            case 2:
                brush = CreateSolidBrush(RGB(0, 0, 255));
                break;
            default:
                brush = CreateSolidBrush(RGB(255, 255, 255));
                break;
            }
            SelectObject(hdc, brush);

            switch (shape)
            {
            case 0:
            {
                POINT p[4] = { drawX + 20, drawY + 0, drawX + 40, drawY + 10,
                    drawX + 20, drawY + 20, drawX + 0, drawY + 10 };
                Polygon(hdc, p, 4);
            }
            break;
            case 1:
                Rectangle(hdc, drawX, drawY, drawX + 40, drawY + 20);
                break;
            case 2:
                Ellipse(hdc, drawX, drawY, drawX + 40, drawY + 20);
                break;
            case 3:
            {
                POINT p[10] = { 15, 10, 20, 3, 25, 10, 35, 13,
                                27, 18, 30, 25, 20, 20, 10, 25,
                                13, 17, 5, 13 };
                for (int i = 0; i < 10; i++) {
                    p[i].x += drawX;
                    p[i].y += drawY;
                }
                Polygon(hdc, p, 10);
            }
            break;
            default:
                break;
            }
        }
        EndPaint(hWnd, &ps);
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

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
