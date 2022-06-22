// InhaWInAPI.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "InhaWInAPI.h"

#include <vector>
#include <string>
#include "Question.h"
#include "GeometricObject.h"
#include "FrameTimer.h"
#include "PhysicsField.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


static TCHAR str[256];
static int count, yPos;
static SIZE size;
static FrameTimer ft;

void TextOutTest( HDC hdc );
void RemoveText( HWND hWnd, HDC hdc, WPARAM wParam );
//void DrawLine_Test( HDC hdc );
void DrawLine( HDC hdc, POINT startPos, POINT endPos );
void DrawGrid( HDC hdc, POINT leftTop, POINT rightBottom, LONG nWidth, LONG nHeight );
//void DrawCircle_Test( HDC hdc );
void DrawCircle( HDC hdc, POINT center, int radius );
//void DrawRect_Test( HDC hdc );
void DrawRect( HDC hdc, POINT center, int width, int height );
void DrawPolygonTest( HDC hdc );


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
    LoadStringW(hInstance, IDC_INHAWINAPI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_INHAWINAPI));

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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_INHAWINAPI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_INHAWINAPI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
    static RECT rcClient;
    static FrameTimer ft;
    static float dt = 0;
    static PhysicsField field;


    switch (message)
    {
    case WM_CREATE:
        GetClientRect( hWnd, &rcClient );
        SetTimer( hWnd, 1, 0, nullptr );
        break;
    case WM_TIMER:
    {
        // Called when Timer On
        dt = ft.Mark();
        if ( (int)dt % 60 == 0 )
        {
            
        }
        field.Update( dt );
        InvalidateRect( hWnd, nullptr, true );


    }
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
        HDC hdc = BeginPaint( hWnd, &ps );
        // TODO: Add any drawing code that uses hdc here...
        
        std::wstring testDT = std::to_wstring( dt );
        TextOut( hdc, 100, 100, testDT.c_str(), testDT.size() );

        field.Draw(hdc);

        EndPaint( hWnd, &ps );
        }
       break;
    case WM_SIZE:
    {
        // ������ â ���� ��
        GetClientRect( hWnd, &rcClient );
    }
    break;
    case WM_KEYDOWN:
    {
        
    }
    break;
    case WM_CHAR:
    {
    }
    break;
    case WM_KEYUP:
    {
    }
    break;

    case WM_LBUTTONDOWN:
    {
        field.AddCircle( {LOWORD(lParam), HIWORD( lParam ) } );
        InvalidateRect( hWnd, nullptr, true );
    }
    break;

    case WM_LBUTTONUP:
    {
    }
    break;

    case WM_MOUSEMOVE:
    {
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

void TextOutTest( HDC hdc )
{
    TextOut( hdc, 100, 100, _T( "Hello World!" ), _tcslen( _T( "Hello World!" ) ) );

    GetTextExtentPoint( hdc, str, _tcslen( str ), &size );
    SetCaretPos( 400 + size.cx, yPos );
    TextOut( hdc, 400, yPos, str, _tcslen( str ) );

    RECT rc;
    rc.left = 200;
    rc.top = 200;
    rc.right = 500;
    rc.bottom = 300;

    // Draw Text at RECT
    SetTextColor( hdc, RGB( 255, 0, 0 ) );
    DrawText( hdc, _T( "Hello World!" ), _tcslen( _T( "Hello World!" ) ), &rc, DT_LEFT );
    SetTextColor( hdc, RGB( 0, 255, 0 ) );
    DrawText( hdc, _T( "Hello World!" ), _tcslen( _T( "Hello World!" ) ), &rc, DT_CENTER );
    SetTextColor( hdc, RGB( 0, 0, 255 ) );
    DrawText( hdc, _T( "Hello World!" ), _tcslen( _T( "Hello World!" ) ), &rc, DT_RIGHT );

}

void RemoveText( HWND hWnd, HDC hdc, WPARAM wParam )
{
    if ( wParam == VK_BACK && count > 0 )
    {
        str[--count] = NULL;
    }
    else if ( wParam == VK_RETURN )
    {
        yPos += 20;
    }
    else
    {
        str[count++] = wParam;
        str[count] = NULL;
    }
    InvalidateRect( hWnd, nullptr, true );
}

void DrawLine_Test( HDC hdc )
{
    MoveToEx( hdc, 200, 200, nullptr );
    LineTo( hdc, 500, 500 );
}

void DrawLine( HDC hdc, POINT startPos, POINT endPos )
{
    MoveToEx( hdc, startPos.x, startPos.y, nullptr );
    LineTo( hdc, endPos.x, endPos.y );
}


// ���ڹ��� �׸���
void DrawGrid( HDC hdc, POINT leftTop, POINT rightBottom, LONG nWidth, LONG nHeight )
{
    for ( int y = leftTop.y; y < rightBottom.y; y += nHeight )
    {
        DrawLine( hdc, { leftTop.x, y }, { rightBottom.x, y } );
    }
    DrawLine( hdc, { rightBottom.x, leftTop.y }, { rightBottom.x, rightBottom.y + 1 } );

    for ( int x = leftTop.x; x < rightBottom.x; x += nWidth )
    {
        DrawLine( hdc, { x, leftTop.y }, { x, rightBottom.y } );
    }
    DrawLine( hdc, { leftTop.x, rightBottom.y }, { rightBottom.x + 1, rightBottom.y } );
}

void DrawCircle( HDC hdc, POINT center, int radius )
{
    Ellipse( hdc, center.x - radius, center.y - radius, center.x + radius, center.y + radius );
}

void DrawRect( HDC hdc, POINT center, int width, int height )
{
    Rectangle( hdc, center.x - width, center.y - height, center.x + width, center.y + width );
}

void DrawPolygonTest( HDC hdc )
{
    const std::vector<POINT> p = {
        {10,150},
        {250,30},
        {500,150},
        {350,300},
        {150,300}
    };
    Polygon( hdc, &p[0], p.size() );
}

void DrawRect_Test( HDC hdc )
{
    Rectangle( hdc, 300, 300, 400, 400 );
}

void DrawCircle_Test( HDC hdc )
{
    Ellipse( hdc, 300, 300, 400, 400 );
}
