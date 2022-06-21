#pragma once

#include "framework.h"
#include "Vec2.h"

class Question
{
public:
	template<typename T>
	static Vec2<T> PointToVec2( const POINT& p )
	{
		return Vec2<T>( (T)p.x, (T)p.y );
	}
	template<typename T>
	static POINT Vec2ToPoint( const Vec2<T>& v )
	{
		return POINT( (int)v.x, (int)v.y );
	}

	static void DrawCircle( HDC hdc, POINT center, int radius );
	static void DrawSunFlower( HDC hdc, POINT center, int radius, int count );
	static void DrawStar(HDC hdc, POINT center, int outerRadius, int count);
	static void DrawStar( HDC hdc, POINT center, int outerRadius, int innerRadius, int count );
private:
};

