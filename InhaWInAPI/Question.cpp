#include "Question.h"
#include "Circle2D.h"
#include <vector>

/*
* Q1.
* ���� ������ �������� ���ڷ� �޾� ���� �׸��� �Լ��� �����϶�
*/

void Question::DrawCircle(HDC hdc, POINT center, int radius)
{
    Ellipse( hdc, center.x - radius, center.y - radius, center.x + radius, center.y + radius );
}

/*
* Q2.
* �عٶ�� �׸��� �Լ��� �����϶�
* ���� �׸��� ���� �⺻ ������ �ܰ��� �׷��� ���� ������ �Է� �޾� �عٶ�� �������� �׷������� �Ѵ�.
*/
void Question::DrawSunFlower( HDC hdc, POINT center, int radius, int count )
{
    if ( count < 3 )
    {
        return;
    }
    DrawCircle( hdc, center, radius );
    Circle2D<int> centerCircle( { center.x, center.y }, radius );
    std::vector<Circle2D<int>> outerCircles(count);
    const double halfRadian = MathSH::PI / (double)count;
    const double radian = halfRadian * 2;
    const double w = radius * std::sin( halfRadian ) * 2;
    const double targetRadius = (radius * w) / ((2 * radius) - w);
    
    for ( int i = 0; i < count; ++i )
    {
        const double curRad = radian * i;
        const Vec2<int> targetCenter(
            (int)((center.x + ((radius + targetRadius) * std::cos( curRad )))),
            (int)((center.x + ((radius + targetRadius) * std::sin( curRad ))))
        );
        outerCircles.emplace_back( targetCenter, targetRadius );
    }

    // Draw
    for ( const auto& e : outerCircles )
    {
        const auto vc = e.GetCenter();
        const POINT c = { vc.x, vc.y };
        DrawCircle( hdc, c, (int)targetRadius );
    }
}

/*
* ���� �׸��� �Լ�
* ���� ������ ���� �ܰ��� ������ �Ÿ��� �Է¹޾� ���� �׸����� �Ѵ�.
*/
void Question::DrawStar( HDC hdc, POINT center, int outerRadius, int count )
{
    const double dTheta = MathSH::PI / count;
    const double innerRadius = outerRadius - (outerRadius / (2 * std::tan( (MathSH::PI / 2) - dTheta )));

    DrawStar( hdc, center, outerRadius, innerRadius, count );
}

void Question::DrawStar( HDC hdc, POINT center, int outerRadius, int innerRadius, int count )
{
    std::vector<POINT> star;
    star.reserve( count * 2 );

    const double dTheta = MathSH::PI / count;
    for ( int i = 0; i < count * 2; i++ )
    {
        const double rad = (i % 2 == 0) ? outerRadius : innerRadius;
        const POINT p = {
            (int)(center.x + rad * cos( i * dTheta )),
            (int)(center.y + rad * sin( i * dTheta ))
        };
        star.push_back( p );
    }

    // Draw
    {
        Polygon( hdc, &star[0], count * 2 );
    }
}


void Question::P93Q7_CreateRect( HDC hdc )
{
    const Vec2<int> centerMiddle( 600, 400 );
    const int rectWidth = 100;
    const int rectHeight = 200;

    Rect<int> rectUp( (centerMiddle + Vec2<int>( 0, -rectHeight )), rectWidth, rectHeight );
    Rect<int> rectDown( (centerMiddle + Vec2<int>( 0, rectHeight )), rectWidth, rectHeight );
    Rect<int> rectLeft( (centerMiddle + Vec2<int>( -rectWidth, 0 )), rectWidth, rectHeight );
    Rect<int> rectRight( (centerMiddle + Vec2<int>( rectWidth, 0 )), rectWidth, rectHeight );

    P93Q7_DrawDirRect( hdc, rectUp, dirUp, L"����" );
    P93Q7_DrawDirRect( hdc, rectDown, dirDown, L"�Ʒ���" );
    P93Q7_DrawDirRect( hdc, rectLeft, dirLeft, L"����" );
    P93Q7_DrawDirRect( hdc, rectRight, dirRight, L"������" );

    std::wstring txt = L"curPos = (" + std::to_wstring( rectPos.x ) + L", " + std::to_wstring( rectPos.y ) + L")";
    TextOut( hdc, 100, 100, txt.c_str(), txt.size() );
}

void Question::P93Q7_GetRectKbdDown( HWND& hWnd, WPARAM wParam )
{
    if ( wParam == VK_RIGHT )
    {
        rectPos = dirRight;
        InvalidateRgn( hWnd, nullptr, true );
    }
    if ( wParam == VK_LEFT )
    {
        rectPos = dirLeft;
        InvalidateRgn( hWnd, nullptr, true );
    }
    if ( wParam == VK_UP )
    {
        rectPos = dirUp;
        InvalidateRgn( hWnd, nullptr, true );
    }
    if ( wParam == VK_DOWN )
    {
        rectPos = dirDown;
        InvalidateRgn( hWnd, nullptr, true );
    }
}

void Question::P93Q7_GetRectKbdUp( HWND& hWnd,WPARAM wParam )
{
    if ( wParam == VK_UP || VK_DOWN || VK_LEFT || VK_RIGHT )
    {
        rectPos = { 0, 0 };
        InvalidateRgn( hWnd, nullptr, true );
    }
}

void Question::P93Q7_DrawDirRect( HDC hdc, const Rect<int>& rect, const Vec2<int>& dir, const std::wstring& str )
{
    if ( rectPos == dir )
    {
        HBRUSH hBrush, oldBrush;
        hBrush = CreateSolidBrush( RGB( 255, 0, 0 ) );
        oldBrush = (HBRUSH)SelectObject( hdc, hBrush );
        rect.Draw( hdc );
        SelectObject( hdc, oldBrush );
        DeleteObject( hBrush );
    }
    else
    {
        RECT r = rect.GetRECT();
        rect.Draw( hdc );
        DrawText( hdc, str.c_str(), str.size(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
    }
}