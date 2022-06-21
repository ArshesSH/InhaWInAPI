#include "Question.h"
#include "Circle2D.h"

#include <vector>

/*
* Q1.
* 원의 중점과 반지름을 인자로 받아 원을 그리는 함수를 구현하라
*/

void Question::DrawCircle(HDC hdc, POINT center, int radius)
{
    Ellipse( hdc, center.x - radius, center.y - radius, center.x + radius, center.y + radius );
}

/*
* Q2.
* 해바라기 그리기 함수를 구현하라
* 원을 그리기 위한 기본 정보에 외곽에 그려질 원의 개수를 입력 받아 해바라기 형식으로 그려지도록 한다.
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
* 별을 그리는 함수
* 별의 중점과 별의 외곽점 까지의 거리를 입력받아 별을 그리도록 한다.
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
