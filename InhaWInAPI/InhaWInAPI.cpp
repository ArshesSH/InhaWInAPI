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


// >> : GDI+

//#include <ObjIdl.h>

#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;



// <<

#pragma comment(lib, "msimg32.lib")


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// >> : GDI+
ULONG_PTR g_GdiToken;
void Gdi_Init();
void Gdi_Draw( HDC hdc );
void Gdi_End();
// << : GDI+

//split window
HWND childHwnd[2];

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
VOID    CALLBACK    TimerProc( HWND, UINT, UINT, DWORD );
BOOL    CALLBACK    DialogProc( HWND, UINT, WPARAM, LPARAM );
BOOL    CALLBACK    Dialog2Proc( HWND, UINT, WPARAM, LPARAM );
BOOL    CALLBACK    Dialog3Proc( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK    ChildWndProc1( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK    ChildWndProc2( HWND, UINT, WPARAM, LPARAM );

// list Ctrl
void MakeColumn( HWND hDlg );
void InsertData( HWND hDlg );


// Created Key State Proc 2022.06.30
//VOID    CALLBACK    KeyStateProc( HWND, UINT, UINT, DWORD );
TCHAR sKeyState[128];

void Update();
Vec2<float> zeroPos = { 200,400 };
bool isLeft = false;


static int count, yPos;
static SIZE size;
static FrameTimer ft;

// for Bitmap
// background img
HBITMAP hBackImage;
BITMAP bitBack;

HBITMAP hSigongImage;
BITMAP bitSigong;

// Animation
HBITMAP hZero;
BITMAP bitZero;
static constexpr int zeroSpriteSizeX = 57;
static constexpr int zeroSpriteSizeY = 52;
int runFrameMax = 0;
int runFrameMin = 0;
int curFrame = runFrameMax;

// Double buffering
HBITMAP hDoubleBufferImage;

FrameTimer timer;

// Dialog 
HWND g_DlgHwnd = NULL;

void CreateBitmap();
void UpdateFrame(HWND hWnd);
void DrawBitmap( HWND hwnd, HDC hdc );
void DrawBitmapDoubleBuffering( HWND hWnd, HDC hdc );
void DeleteBitmap();

// Finish BItmap

void DrawRectText( HDC hdc );
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


    Gdi_Init();
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
        else
        {
            Update();
        }
    }
    Gdi_End();


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



void Gdi_Init()
{
    GdiplusStartupInput gpsi;
    GdiplusStartup( &g_GdiToken, &gpsi, nullptr );
}

void Gdi_Draw( HDC hdc )
{
    Graphics graphics( hdc );

    // Txt
    SolidBrush brush( Color( 255, 255, 0, 0 ) );
    FontFamily fontFamily( L"Consolas" );
    Font font( &fontFamily, 24, FontStyleRegular, UnitPixel );
    PointF pointF( 10.0f, 20.0f );

    graphics.DrawString( L"Hello World!", -1, &font, pointF, &brush );


    // Line
    Pen pen( Color( 100, 0, 255, 255 ), 6 );
    graphics.DrawLine( &pen, 0, 0, 200, 200 );

    // Image
    Image image( L"Images/sigong.png" );
    int w = image.GetWidth();
    int h = image.GetHeight();
    graphics.DrawImage( &image, 300, 100, w, h );

    // animation
    Image zeroImage( L"Images/zero_run.png" );
    int wz = zeroImage.GetWidth() / 16;
    int hz = zeroImage.GetHeight() / 2;
    int xStart = curFrame * wz;
    int yStart = 0;
    // deledte chroma
    ImageAttributes imgAttr;
    imgAttr.SetColorKey( Color( 245, 0, 245 ), Color( 255, 10, 255 ) );
    graphics.DrawImage( &zeroImage, Gdiplus::Rect( 400, 100, wz * 2, hz * 2 ), xStart, yStart, wz, hz, UnitPixel, &imgAttr );


    // alpha rect
    brush.SetColor( Color( 128, 255, 0, 0 ) );
    graphics.FillRectangle( &brush, 600, 100, wz * 2, hz * 2 );


    // Rotate Image
    Image* pImg;
    pImg = Image::FromFile( L"Images/sigong.png" );
    POINT sigongPos = { 300, 200 };
    if ( pImg != nullptr )
    {
        w = pImg->GetWidth();
        h = pImg->GetHeight();

        Gdiplus::Matrix mat;
        static int rot = 0;

        mat.RotateAt( (rot % 360),
            Gdiplus::PointF( ((float)sigongPos.x + (float)w * 0.5f), (float)sigongPos.y + (float)(h * 0.5f) )
        );
        graphics.SetTransform( &mat );
        graphics.DrawImage( pImg, sigongPos.x, sigongPos.y, w, h );
        rot -= 50;

        mat.Reset();
        graphics.SetTransform( &mat );
    }

    // Alpha Image
    if ( pImg )
    {
        REAL transparency = 0.5f;
        ColorMatrix colorMatrix =
        {
            1.0f, 0.0f, 0.0f, 0.0f,         0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,         0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,         0.0f,
            0.0f, 0.0f, 0.0f, transparency, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,         1.0f
        };
        imgAttr.SetColorMatrix( &colorMatrix );
        sigongPos.x = 400;
        graphics.DrawImage( pImg, Gdiplus::Rect( sigongPos.x, sigongPos.y, w, h ),  // Dest coord
            0, 0, w, h, UnitPixel, &imgAttr );                                      // Src Coord
    }


    // GrayScale
    if ( pImg )
    {
        ColorMatrix grayMatrix =
        {
            0.3f, 0.3f, 0.3f,        0.0f,         0.0f,
            0.3f, 0.3f, 0.3f,        0.0f,         0.0f,
            0.3f, 0.3f, 0.3f,        0.0f,         0.0f,
            0.0f, 0.0f, 0.0f,        1.0f,         0.0f,
            0.0f, 0.0f, 0.0f,        0.0f,         1.0f
        };
        imgAttr.SetColorMatrix( &grayMatrix );
        sigongPos.x = 500;
        graphics.DrawImage( pImg, Gdiplus::Rect( sigongPos.x, sigongPos.y, w, h ),  // Dest coord
            0, 0, w, h, UnitPixel, &imgAttr );
    }

    // test
    if ( pImg != nullptr)
    {
        ColorMatrix grayMatrix =
        {
            0.3f, 0.3f, 0.3f,        0.0f,         0.0f,
            0.3f, 0.3f, 0.3f,        0.0f,         0.0f,
            0.3f, 0.3f, 0.3f,        0.0f,         0.0f,
            0.0f, 0.0f, 0.0f,        1.0f,         0.0f,
            0.0f, 0.0f, 0.0f,        0.0f,         1.0f
        };
        imgAttr.SetColorMatrix( &grayMatrix );
        sigongPos.x = 600;

        pImg->RotateFlip( RotateNoneFlipX );
        graphics.DrawImage( pImg, Gdiplus::Rect( sigongPos.x, sigongPos.y, w, h ),  // Dest coord
            0, 0, w, h, UnitPixel, &imgAttr );
    }

    delete pImg;
}

void Gdi_End()
{
    GdiplusShutdown( g_GdiToken );
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

    // Split winodws
    wcex.lpfnWndProc = ChildWndProc1;
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = _T( "ChildWnd1Class" );
    RegisterClassEx( &wcex );

    wcex.lpfnWndProc = ChildWndProc2;
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = _T( "ChildWnd2Class" );
    RegisterClassEx( &wcex );

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
    //static Question q7;

    static Circle<int> c1( { 30, 30 }, 20 );
    static Circle<int> c2( { 100,100 }, 30 );
    static POINT startPos;
    static POINT curPos;
    static bool bDrag = false;
    static int selectedMenu;
    static char filepath[100], filename[100];



    switch (message)
    {
    case WM_CREATE:
        GetClientRect( hWnd, &rcClient );

        childHwnd[0] = CreateWindowEx(
            WS_EX_CLIENTEDGE, _T( "ChildWnd1Class" ), NULL, WS_CHILD | WS_VISIBLE,
            0, 0, rcClient.right, rcClient.bottom * 0.5 - 1,
            hWnd, nullptr, hInst, nullptr
        );

        childHwnd[1] = CreateWindowEx(
            WS_EX_CLIENTEDGE, _T( "ChildWnd2Class" ), NULL, WS_CHILD | WS_VISIBLE,
            0, rcClient.bottom*0.5 + 1, rcClient.right, rcClient.bottom * 0.5 - 1,
            hWnd, nullptr, hInst, nullptr
        );

        // Create Bitmap
        CreateBitmap();
        //SetTimer( hWnd, 1, 200, KeyStateProc );
        SetTimer( hWnd, 2, 50, TimerProc );
        /*
        // Create Timer
        SetTimer( hWnd, 1, 100, nullptr );
        SetTimer( hWnd, 2, 200, nullptr );
        */


        break;
    case WM_TIMER:
    {
        // Called when Timer On

        /*
        *         // Is Timer1
        if ( wParam == 1 )
        {
            curPos.x += 40;
            InvalidateRect( hWnd, nullptr, true );
        }
        else if ( wParam == 2 )
        {
            curPos.x -= 40;
            InvalidateRect( hWnd, nullptr, true );
        }
        */

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

        //DrawBitmap(hWnd, hdc );
        DrawBitmapDoubleBuffering( hWnd, hdc );
        DrawRectText( hdc );

        EndPaint( hWnd, &ps );
        }
       break;
    case WM_SIZE:
    {
        // 윈도우 창 변경 시
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

    }
    break;

    case WM_RBUTTONDOWN:
    {
        DialogBox( hInst, MAKEINTRESOURCE( IDD_DIALOG1 ), hWnd, DialogProc);
    }
    break;

    case WM_MBUTTONDOWN:
        {
            //DialogBox( hInst, MAKEINTRESOURCE( IDD_DIALOG2 ), hWnd, Dialog2Proc );
            if ( !IsWindow( g_DlgHwnd ) )
            {
                g_DlgHwnd = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG2 ), hWnd, Dialog2Proc );
                ShowWindow( g_DlgHwnd, SW_SHOW );
            }
        }
        break;
    case WM_LBUTTONUP:
    {
            DialogBox( hInst, MAKEINTRESOURCE( IDD_DIALOG3 ), hWnd, Dialog3Proc );
    }
    break;

    case WM_MOUSEMOVE:
    {
    }
    break;

    case WM_DESTROY:

        // Destroy Bitmap
        DeleteBitmap();



        //Destroy Timer
        //KillTimer( hWnd, 1 );
        KillTimer( hWnd, 2 );
        
        
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

void Update()
{
    DWORD newTime = GetTickCount64();
    static DWORD oldTime = newTime;
    if ( newTime - oldTime < 100 )
    {
        return;
    }
    oldTime = newTime;

    if ( GetAsyncKeyState( VK_RIGHT ) & 0x8000 )
    {
        isLeft = false;
        zeroPos.x += 20;
    }
    if ( GetAsyncKeyState( VK_LEFT ) & 0x8000 )
    {
        isLeft = true;
        zeroPos.x -= 20;
    }
    if ( GetAsyncKeyState( VK_DOWN ) & 0x8000 )
    {
        zeroPos.y += 20;
    }
    if ( GetAsyncKeyState( VK_UP ) & 0x8000 )
    {
        zeroPos.y -= 20;
    }

}

VOID CALLBACK TimerProc( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime )
{
    UpdateFrame( hWnd );
    InvalidateRect( hWnd, nullptr, false );
}


BOOL CALLBACK DialogProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
    static int Check[3], Radio;
    TCHAR hobby[][30] = { _T( "독서" ), _T( "음악감상" ), _T( "게임" ) };
    TCHAR gender[][30] = { _T( "여성" ), _T( "남성" )};
    TCHAR output[258];


    UNREFERENCED_PARAMETER( lParam );
    switch ( iMsg )
    {
    case WM_INITDIALOG:
    {
        HWND hBtn = GetDlgItem( hDlg, IDC_PAUSE );
        EnableWindow( hBtn, FALSE );

        // Radio
        CheckRadioButton( hDlg, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1 );
    }
        return TRUE;

    case WM_COMMAND:
        switch ( LOWORD( wParam ) )
        {
            // Radio button
        case IDC_CHECK1:
            {
                Check[0] = 1 - Check[0];
            }
            break;

        case IDC_CHECK2:
        {
                Check[1] = 1 - Check[1];
        }
        break;
        case IDC_CHECK3:
        {
                Check[2] = 1 - Check[2];
        }
        break;
        case IDC_RADIO1:
            Radio = 0;
            break;
        case IDC_RADIO2:
            Radio = 1;
            break;
        case IDC_BUTTON_OUTPUT:
            _stprintf_s( output, _T( "선택한 취미는 %s %s %s 입니다.\r\n" ) _T( "선택한 성별은 %s입니다.\n" ),
                Check[0] ? hobby[0] : _T( "" ),
                Check[1] ? hobby[1] : _T( "" ),
                Check[2] ? hobby[2] : _T( "" ),
                gender[Radio]
            );
            SetDlgItemText( hDlg, IDC_EDIT_OUTPUT, output );
            break;
  

        case IDC_BUTTON_COPY:
        {
            TCHAR word[128];
            GetDlgItemText( hDlg, IDC_EDIT_SRC, word, 256 );
            SetDlgItemText( hDlg, IDC_EDIT_COPY, word );
        }
            break;
        case IDC_BUTTON_CLEAR:
        {
            SetDlgItemText( hDlg, IDC_EDIT_SRC, _T(""));
            SetDlgItemText( hDlg, IDC_EDIT_COPY, _T( "" ));
        }
            break;
        case IDC_START:
        {
            HWND hBtn = GetDlgItem( hDlg, IDC_START );
            EnableWindow( hBtn, FALSE );
            hBtn = GetDlgItem( hDlg, IDC_PAUSE );
            EnableWindow( hBtn, TRUE );

            SetDlgItemText( hDlg, IDC_TEXT, _T( "시작" ) );
        }
            break;
        case IDC_PAUSE:
        {
            HWND hBtn = GetDlgItem( hDlg, IDC_START );
            EnableWindow( hBtn, TRUE );
            hBtn = GetDlgItem( hDlg, IDC_PAUSE );
            EnableWindow( hBtn, FALSE );
            SetDlgItemText( hDlg, IDC_TEXT, _T( "정지" ) );
        }
            break;
        case IDC_CLOSE:
        {
            EndDialog( hDlg, LOWORD( wParam ) );
            return TRUE;
        }
            break;
        case IDC_BUTTON_PRINT:
        {
            HDC hdc = GetDC( hDlg );

            TextOut( hdc, 10, 10, _T( "Hello Button!" ), 13 );
            SetDlgItemText( hDlg, IDC_TEXT, _T( "정말 의미있는 정보" ) );
            ReleaseDC( hDlg, hdc );
        }

        break;
        case IDOK:
        case IDCANCEL:
        {
            EndDialog( hDlg, LOWORD( wParam ) );
            return TRUE;
        }
        break;

        break;
        }
    }
    return FALSE;
}

BOOL CALLBACK Dialog2Proc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
    static HWND hCombo;
    static HWND hList;

    static int selection;
    TCHAR name[20];


    UNREFERENCED_PARAMETER( lParam );
    switch ( iMsg )
    {
    case WM_INITDIALOG:
        {
            hCombo = GetDlgItem( hDlg, IDC_COMBO_LIST );
            hList = GetDlgItem( hDlg, IDC_LIST_NAME );
        }
        return TRUE;

    case WM_COMMAND:
        switch ( LOWORD( wParam ) )
        {
        case IDC_BUTTON_INSERT:
            {
                GetDlgItemText( hDlg, IDC_EDIT_NAME, name, 20 );
                if ( _tcscmp( name, _T( "" ) ) )
                {
                    SendMessage( hCombo, CB_ADDSTRING, 0, (LPARAM)name );
                    SendMessage( hList, LB_ADDSTRING, 0, (LPARAM)name );
                    SetDlgItemText( hDlg, IDC_EDIT_NAME, _T( "" ) );
                }
                return TRUE;
            }
            break;
        case IDC_BUTTON_DELETE:
            {
                SendMessage( hCombo, CB_DELETESTRING, selection, 0 );
                SendMessage( hList, LB_DELETESTRING, selection, 0 );
                return TRUE;
            }
            break;
        case IDC_COMBO_LIST:
            {
                if ( HIWORD( wParam ) == CBN_SELCHANGE )
                {
                    selection = SendMessage( hCombo, CB_GETCURSEL, 0, 0 );
                }
            }
            break;
        case IDC_LIST_NAME:
            {
                if ( HIWORD( wParam ) == LBN_SELCHANGE )
                {
                    selection = SendMessage( hCombo, LB_GETCURSEL, 0, 0 );
                }
            }
            break;
        case IDCLOSE:
        case IDOK:
        case IDCANCEL:
            {
                //EndDialog( hDlg, LOWORD( wParam ) );
                // for modeless
                DestroyWindow( g_DlgHwnd );
                g_DlgHwnd = NULL;
                return TRUE;
            }
            break;
        }
        break;
    }

    return FALSE;
}

BOOL CALLBACK Dialog3Proc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{

    UNREFERENCED_PARAMETER( lParam );
    switch ( iMsg )
    {
    case WM_INITDIALOG:
        {
            MakeColumn( hDlg );
        }
        return TRUE;

    case WM_COMMAND:
        switch ( LOWORD( wParam ) )
        {
        case IDC_BUTTON_INPUT_DATA:
            {
                InsertData( hDlg );
            }
            break;
        case IDOK:
        case IDCANCEL:
            {
                EndDialog( hDlg, LOWORD( wParam ) );
                hDlg = NULL;
                return TRUE;
            }
            break;
        }
        break;
    }
    return FALSE;
}

static constexpr int IDC_CHILD_BTN = 100;

LRESULT CALLBACK ChildWndProc1( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch ( message )
    {
    case WM_CREATE:
       
        break;
    case WM_COMMAND:
        break;
    case WM_MOUSEMOVE:
        //InvalidateRect( hWnd, nullptr, false );
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint( hWnd, &ps );

            DrawBitmapDoubleBuffering( hWnd, hdc );
            DrawRectText( hdc );

            EndPaint( hWnd, &ps );
        }
        break;
    case WM_DESTROY:
        break;
    }

    // 처리되지 않은 메시지를 다시 돌려주겠다는 뜻
    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK ChildWndProc2( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    static POINT ptMouse;
    static HWND hBtn;
    static bool bToggle = false;

    HDC hdc;
    HANDLE hFile;
    TCHAR InBuff[1000];
    TCHAR OutBuff[100] = _T( "텍스트" );
    DWORD size;
    RECT rt;

    switch ( message )
    {
    case WM_CREATE:
        hBtn = CreateWindow( _T( "button" ), _T( "OK" ), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            300, 10, 100, 30, hWnd, (HMENU)IDC_CHILD_BTN, hInst, NULL );
        break;
    case WM_COMMAND:
        switch ( LOWORD( wParam ) )
        {
        case IDC_CHILD_BTN:
            {
                bToggle = !bToggle;
            }
            break;
        }
        break;
    case WM_MOUSEMOVE:
        
        //InvalidateRect( hWnd, nullptr, false );
        break;

    case WM_LBUTTONDOWN:
        {
            hFile = CreateFile(
                _T( "test1.txt" ),
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL, OPEN_EXISTING, 0, 0
            );
            memset( InBuff, 0, sizeof( InBuff ) );
            ReadFile( hFile, InBuff, 999 * sizeof( TCHAR ), &size, NULL );
            hdc = GetDC( hWnd );
            GetClientRect( hWnd, &rt );
            DrawText( hdc, InBuff, (int)_tcslen( InBuff ), &rt, DT_TOP | DT_LEFT );
            ReleaseDC( hWnd, hdc );
            WriteFile( hFile, OutBuff, (DWORD)_tcslen( OutBuff ) * sizeof( TCHAR ), &size, NULL );
            CloseHandle( hFile );
        }
        break;

    case WM_PAINT:
        {
            GetCursorPos( &ptMouse );

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint( hWnd, &ps );

            TCHAR str[128];
            wsprintf( str, TEXT( "World Position : ( %04d, %04d)" ), ptMouse.x, ptMouse.y );
            TextOut( hdc, 10, 30, str, lstrlen( str ) );

            // Local Pos로 변경
            ScreenToClient( hWnd, &ptMouse );
            wsprintf( str, TEXT( "Local Position : ( %04d, %04d)" ), ptMouse.x, ptMouse.y );
            TextOut( hdc, 10, 50, str, lstrlen( str ) );

            if ( bToggle )
            {
                TextOut( hdc, 300, 100, _T( "Button Clicked" ), 14 );
            }

            //DrawBitmapDoubleBuffering( hWnd, hdc );
            //DrawRectText( hdc );

            EndPaint( hWnd, &ps );
        }
        break;
    case WM_DESTROY:
        break;
    }

    // 처리되지 않은 메시지를 다시 돌려주겠다는 뜻
    return DefWindowProc( hWnd, message, wParam, lParam );
}

void MakeColumn( HWND hDlg )
{
    LPCTSTR     name[2] = { _T( "이름" ), _T( "전화번호" ) };
    LVCOLUMN    lvCol = { 0, };
    HWND        hList;
    int         i;
    hList = GetDlgItem( hDlg, IDC_LIST_MEMBER );
    lvCol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT| LVCF_SUBITEM;
    lvCol.fmt = LVCFMT_LEFT;

    RECT rt;
    GetClientRect( hList, &rt );

    for ( i = 0; i < 2; ++i )
    {
        lvCol.cx = (int)rt.right * 0.5;
        lvCol.iSubItem = i;
        lvCol.pszText = (LPWSTR)name[i];
        SendMessage( hList, LVM_INSERTCOLUMN, (WPARAM)i, (LPARAM)&lvCol );
    }
}

void InsertData( HWND hDlg )
{
    LVITEM      item;
    HWND        hList;
    int cnt;
    //LPCTSTR     name[20] = { _T( "이우석" ), _T( "날개뼈" ) };
    //LPCTSTR     phone[20] = { _T( "010-1234-5678" ), _T( "02-123-2345" ) };
    TCHAR       name[20], phone[20];


    GetDlgItemText( hDlg, IDC_EDIT_MEMBER_NAME, name, 20 );
    SetDlgItemText( hDlg, IDC_EDIT_MEMBER_NAME, _T( "" ));

    if ( _tcscmp( name, _T( "" ) ) == 0 )
    {
        return;
    }

    GetDlgItemText( hDlg, IDC_EDIT_MEMBER_PHONE, phone, 20 );
    SetDlgItemText( hDlg, IDC_EDIT_MEMBER_PHONE, _T( "" ) );

    hList = GetDlgItem( hDlg, IDC_LIST_MEMBER );
    cnt = ListView_GetItemCount( hList );
    item.mask = LVIF_TEXT;
    item.iItem = cnt;
    item.iSubItem = 0;
    item.pszText = (LPWSTR)name;
    ListView_InsertItem( hList, &item );
    ListView_SetItemText( hList, cnt, 1, (LPWSTR)phone );


    //for ( int i = 0; i < 2; ++i )
    //{
    //    item.mask = LVIF_TEXT;
    //    item.iItem = i;
    //    item.iSubItem = 0;
    //    item.pszText = (LPWSTR)name[i];
    //    ListView_InsertItem( hList, &item );
    //    ListView_SetItemText( hList, i, 1, (LPWSTR)phone[i] );
    //}
}

// Key capture
VOID CALLBACK KeyStateProc( HWND, UINT, UINT, DWORD )
{
    // 0x8000 = key down
    if ( GetKeyState( 'A' ) & 0x8000 )
    {
        wsprintf( sKeyState, TEXT( "%s" ), _T( "A-Key pressed" ) );
    }
    else if ( GetKeyState( 'D' ) & 0x8000)
    {
        wsprintf( sKeyState, TEXT( "%s" ), _T( "D-Key pressed" ) );
    }
    else
    {
        wsprintf( sKeyState, TEXT( "" ) );
    }
}




void CreateBitmap()
{
    hBackImage = (HBITMAP)LoadImage( NULL, L"Images/수지.bmp" , IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION );
    GetObject( hBackImage, sizeof( BITMAP ), &bitBack );

    // for Trans blt
    hSigongImage = (HBITMAP)LoadImage( NULL, L"Images/sigong.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION );
    GetObject( hSigongImage, sizeof( BITMAP ), &bitSigong );

    // for animation
    hZero = (HBITMAP)LoadImage( NULL, L"Images/zero_run.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION );
    GetObject( hZero, sizeof( BITMAP ), &bitZero );
    runFrameMax = bitZero.bmWidth / zeroSpriteSizeX - 1;
    runFrameMin = 2;
    curFrame = runFrameMin; 

}

void UpdateFrame( HWND hWnd )
{
    curFrame++;
    if ( curFrame > runFrameMax )
    {
        curFrame = runFrameMin;
    }

}

void DrawBitmap( HWND hwnd, HDC hdc )
{
    HDC hMemDC;
    HBITMAP hOldBitmap;
    int bx, by;

    hMemDC = CreateCompatibleDC( hdc );
    hOldBitmap = (HBITMAP)SelectObject( hMemDC, hBackImage );
    bx = bitBack.bmWidth;
    by = bitBack.bmHeight;

    BitBlt( hdc, 0, 0, bx, by, hMemDC, 0, 0, SRCCOPY );
    //StretchBlt( hdc, 200, 200, bx * 2, by * 2, hMemDC, 0, 0, bx, by, SRCCOPY );

    SelectObject( hMemDC, hOldBitmap );
    DeleteDC( hMemDC );


    // sigong
    hMemDC = CreateCompatibleDC( hdc );
    hOldBitmap = (HBITMAP)SelectObject( hMemDC, hSigongImage );
    bx = bitSigong.bmWidth;
    by = bitSigong.bmHeight;
    TransparentBlt( hdc, 200, 200, bx, by, hMemDC, 0, 0, bx, by, RGB( 255, 0, 255 ) );

    SelectObject( hMemDC, hOldBitmap );
    DeleteDC( hMemDC );

    // Zero
    hMemDC = CreateCompatibleDC( hdc );
    hOldBitmap = (HBITMAP)SelectObject( hMemDC, hZero );
    bx = zeroSpriteSizeX;
    by = zeroSpriteSizeY;

    int xStart = curFrame * bx;
    int yStart = 0;

    TransparentBlt( hdc, 200, 400, bx*2, by*2, hMemDC, xStart, yStart, bx, by, RGB( 255, 0, 255 ) );

    SelectObject( hMemDC, hOldBitmap );
    DeleteDC( hMemDC );
}

void DrawBitmapDoubleBuffering( HWND hWnd, HDC hdc )
{
    HDC hMemDC;
    HBITMAP hOldBitmap;

    HDC hMemDC2;
    HBITMAP hOldBitmap2;

    int bx, by;

    hMemDC = CreateCompatibleDC( hdc );

    if ( hDoubleBufferImage == NULL )
    {
        // Create Bitmap Image for Double buffering
        hDoubleBufferImage = CreateCompatibleBitmap( hdc, rcClient.right, rcClient.bottom );
    }

    hOldBitmap = (HBITMAP)SelectObject( hMemDC, hDoubleBufferImage );

    // Background Image
    {
        hMemDC2 = CreateCompatibleDC( hMemDC );
        hOldBitmap2 = (HBITMAP)SelectObject( hMemDC2, hBackImage );
        bx = bitBack.bmWidth;
        by = bitBack.bmHeight;

        BitBlt( hMemDC, 0, 0, bx, by, hMemDC2, 0, 0, SRCCOPY );
        //StretchBlt( hMemDC, 900, 0, bx * 2, by * 2, hMemDC2, 200, 200, bx, by, SRCCOPY );


        SelectObject( hMemDC2, hOldBitmap2 );
        DeleteDC( hMemDC2 );
    }

    // sigong
    {
        hMemDC2 = CreateCompatibleDC( hMemDC );
        hOldBitmap2 = (HBITMAP)SelectObject( hMemDC2, hSigongImage );
        bx = bitSigong.bmWidth;
        by = bitSigong.bmHeight;
        TransparentBlt( hMemDC, 200, 200, bx, by, hMemDC2, 0, 0, bx, by, RGB( 255, 0, 255 ) );

        SelectObject( hMemDC2, hOldBitmap2 );
        DeleteDC( hMemDC2 );
    }

    // Zero
    {

        hMemDC2 = CreateCompatibleDC( hMemDC );
        hOldBitmap2 = (HBITMAP)SelectObject( hMemDC2, hZero );
        bx = zeroSpriteSizeX;
        by = zeroSpriteSizeY;

        int xStart = curFrame * bx;
        int yStart = 0;

        if ( !isLeft )
        {
            TransparentBlt( hMemDC, zeroPos.x, zeroPos.y, bx * 2, by * 2, hMemDC2, xStart, yStart, bx, by, RGB( 255, 0, 255 ) );
        }

        if ( isLeft )
        {
            xStart = (runFrameMax * bx) - xStart;
            yStart = by;
            TransparentBlt( hMemDC, zeroPos.x, zeroPos.y, bx * 2, by * 2, hMemDC2, xStart, yStart, bx, by, RGB( 255, 0, 255 ) );
        }

        SelectObject( hMemDC2, hOldBitmap2 );
        DeleteDC( hMemDC2 );
    }

    // >> : Gdi+
    Gdi_Draw( hMemDC );

    // << : Gdi+

    // MEmDC2 => memdc
    BitBlt( hdc, 0, 0, rcClient.right, rcClient.bottom, hMemDC, 0, 0, SRCCOPY );
    SelectObject( hMemDC, hOldBitmap );
    DeleteDC( hMemDC );
}

void DeleteBitmap()
{
    DeleteObject( hBackImage );
    DeleteObject( hSigongImage );
    DeleteObject( hZero );
}

void DrawRectText( HDC hdc )
{
    static int yPos = 0;
    std::wstring str = L"이미지 출력";
    TextOut( hdc, 10, yPos, str.c_str(), str.size() );
    yPos += 5;
    if ( yPos > rcClient.bottom )
    {
        yPos = 0;
    }

    TextOut( hdc, 50, 10, sKeyState, _tcslen( sKeyState ) );
}

void TextOutTest( HDC hdc )
{
    TextOut( hdc, 100, 100, _T( "Hello World!" ), _tcslen( _T( "Hello World!" ) ) );
    TCHAR str[100];
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
    TCHAR str[100];
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


// 격자무늬 그리기
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

void OutFromFile( TCHAR filename[], const HWND& hwnd )
{
    FILE* pF;
    HDC hdc;
    int line = 0;
    std::wstring buf;
    TCHAR buffer[500];
    hdc = GetDC( hwnd );
#ifdef _UNICODE
    _tfopen_s( &pF, filename, _T( "r, ccs = UNICODE" ) );
#else
    _tfopen_s( &pF, filename, _T( "r" ) );
#endif // _UNICODE
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