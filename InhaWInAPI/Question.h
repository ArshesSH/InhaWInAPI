#pragma once

#include "framework.h"
#include "GeometricObject.h"
#include "Vec2.h"
#include <string>

class Question
{
public:

	static void DrawCircle( HDC hdc, POINT center, int radius );
	static void DrawSunFlower( HDC hdc, POINT center, int radius, int count );
	static void DrawStar(HDC hdc, POINT center, int outerRadius, int count);
	static void DrawStar( HDC hdc, POINT center, int outerRadius, int innerRadius, int count );
	void P93Q7_CreateRect( HDC hdc );
	void P93Q7_GetRectKbdDown( HWND& hWnd, WPARAM wParam );
	void P93Q7_GetRectKbdUp( HWND& hWnd, WPARAM wParam );
	void P93Q7_DrawDirRect( HDC hdc, const Rect<int>& rect, const Vec2<int>& dir, const std::wstring& str );
private:
	const Vec2<int> dirUp = { 0,-1 };
	const Vec2<int> dirDown = { 0, 1 };
	const Vec2<int> dirLeft = { -1, 0 };
	const Vec2<int> dirRight = { 1 , 0 };
	Vec2<int> rectPos = { 0, 0 };
};

