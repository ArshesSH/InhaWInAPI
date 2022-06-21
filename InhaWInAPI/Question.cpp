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
