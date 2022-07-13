// InhaWInAPI.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "InhaWInAPI.h"

#include <vector>
#include <string>
#include "Question.h"
#include "GeometricObject.h"
#include "FrameTimer.h"

#include <commdlg.h>

// List Control
#include <CommCtrl.h>


#pragma comment(lib, "msimg32.lib")


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

void FileRead( HWND hWnd, const wchar_t* filename );
void FileSave( HWND hWnd, const wchar_t* filename );

// Created Key State Proc 2022.06.30
//VOID    CALLBACK    KeyStateProc( HWND, UINT, UINT, DWORD );
TCHAR sKeyState[128];

// Dialog 
HWND g_DlgHwnd = NULL;

void OutFromFile( TCHAR filename[], const HWND& hwnd );

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

    while ( true )
    {
        if ( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
        {
            if ( msg.message == WM_QUIT )
            {
                break;
            }
            else
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
        }
    }


    //// Main message loop:
    //while (GetMessage(&msg, nullptr, 0, 0))
    //{
    //    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    //    {
    //        TranslateMessage(&msg);
    //        DispatchMessage(&msg);
    //    }
    //}

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

    RegisterClassExW(&wcex);

    return NULL;
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

static RECT rcClient;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hEdit;
    RECT rect;

    switch (message)
    {
    case WM_CREATE:
        GetClientRect( hWnd, &rect );
        hEdit = CreateWindow( _T( "edit" ), nullptr,
            WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE,
            0, 100, rect.right, rect.bottom, hWnd, (HMENU)0, hInst, nullptr
        );
    case WM_SIZE:
        GetClientRect( hWnd, &rect );
        MoveWindow( hEdit, 0, 0, rect.right, rect.bottom, TRUE );
        break;
        
        break;
    case WM_TIMER:
    {

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
            case ID_FILE_OPEN:
                {
                    FileRead( hEdit, _T( "test2.txt" ) );
                }
                break;
            case ID_FILE_SAVE:
                {
                    FileSave( hEdit, _T( "test2.txt" ) );
                }
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

            EndPaint( hWnd, &ps );
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

    }
    break;

    case WM_RBUTTONDOWN:
    {
    }
    break;

    case WM_MBUTTONDOWN:
        {
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

        // Destroy Bitmap


        
        PostQuitMessage( 0 );

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

void FileRead( HWND hWnd, const wchar_t* filename )
{
    HANDLE hFile;
    LPTSTR buffer;
    DWORD size, charNum;
    int fileSize;
    hFile = CreateFile( filename, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return;
    }
    fileSize = GetFileSize( hFile, &size );
    const int cntChar = (int)fileSize / sizeof( TCHAR );
    fileSize = GetFileSize( hFile, &size );
    buffer = new TCHAR[cntChar + 1];
    memset( buffer, 0, fileSize );
    ReadFile( hFile, buffer, fileSize, &charNum, NULL );
    buffer[cntChar] = NULL;
    SetWindowText( hWnd, buffer );
    delete[] buffer ;
    CloseHandle( hFile );
}

void FileSave( HWND hWnd, const wchar_t* filename )
{
    HANDLE hFile;
    LPTSTR buffer;
    int size;

#ifdef _UNICODE
    WORD uni = 0xFEFF;
    DWORD nSize;
#endif // _UNICODE
    hFile = CreateFile( filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );

#ifdef _UNICODE
    WriteFile( hFile, &uni, 2, &nSize, NULL );

#endif // _UNICODE
    size = GetWindowTextLength( hWnd );
    buffer = new TCHAR[size + 1];
    memset( buffer, 0, (size + 1) * sizeof( TCHAR ) );
    size = GetWindowText( hWnd, (LPTSTR)buffer, size + 1 );
    buffer[size] = NULL;
    WriteFile( hFile, buffer, size * sizeof( TCHAR ), (LPDWORD)&size, NULL );
    delete[] buffer;
    CloseHandle( hFile );
}


void OutFromFile( TCHAR filename[], const HWND& hwnd )
{
    FILE* pF;
    HDC hdc;
    int line = 0;
    std::wstring buf;
    TCHAR buffer[500];
    hdc = GetDC( hwnd );
   // _tfopen_s( &pF, filename, _T( "r, ccs = UNICODE" ) );
    _tfopen_s( &pF, filename, _T( "r" ) );
    while ( _fgetts( buffer, 100, pF ) != NULL )
    {
        const auto bufferSize = _tcslen( buffer ) - 1;
        if ( buffer[bufferSize] == _T( '\n' ) )
        {
            buffer[bufferSize] = NULL;
            TextOut( hdc, 0, line * 20, buffer, _tcslen( buffer ) );
            line++;
        }
        fclose( pF );
        ReleaseDC( hwnd, hdc );
    }
}