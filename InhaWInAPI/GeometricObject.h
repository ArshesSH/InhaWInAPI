#pragma once

#include "Vec2.h"
#include "MathSH.h"
#include "framework.h"
#include <vector>

template<typename T>
class GeometricObject
{
public:
	GeometricObject()
		:
		center((T)0, (T)0)
	{}
	GeometricObject(const Vec2<T> center)
		:
		center(center)
	{}
	GeometricObject( T x, T y )
		:
		center(x, y)
	{}
	virtual ~GeometricObject() {}

	virtual bool IsOverlapWith( const GeometricObject<T>& ) const { exit( 1 ); }
	virtual bool IsContainedBy( const GeometricObject<T>& ) const { exit( 1 ); }
	virtual bool IsContains( const GeometricObject<T>& ) const { exit( 1 ); }
	virtual bool IsContains( const Vec2<T>& ) const { exit( 1 ); }
	virtual bool IsContains( T, T ) const { exit( 1 ); }
	virtual double GetArea() const { exit( 1 ); };
	virtual double GetPerimeter() const { exit( 1 ); }
	double GetDistanceWith( const GeometricObject<T>& other ) const
	{
		const Vec2<T> vDist = other.center - center;
		return vDist.GetLength();
	}
	Vec2<T> GetCenter() const
	{
		return center;
	}
	T GetCenterX() const
	{
		return center.x;
	}
	T GetCenterY() const
	{
		return center.y;
	}
	virtual void SetCenter( const Vec2<T>& p )
	{
		center = p;
	}
	virtual void SetCenter( T x, T y )
	{
		center = { x, y };
	}
	virtual void Draw( HDC hdc ) const = 0;
	POINT Vec2ToPoint( const Vec2<T>& v ) const
	{
		return POINT( (int)v.x, (int)v.y );
		
	}
	Vec2<T> PointToVec2( const POINT& p ) const
	{
		return Vec2<T>( (T)p.x, (T)p.y );
	}
	bool GetSelected() const
	{
		return isSelected;
	}
	void SetSelected( bool parm = true )
	{
		isSelected = parm;
	}

protected:
	Vec2<T> center;
	bool isSelected = false;
};

template<typename T>
class Circle : public GeometricObject<T>
{
public:
	Circle() {}
	Circle(const Vec2<T>& center, T radius )
		:
		GeometricObject<T>(center),
		radius(radius)
	{}
	Circle(T x, T y, T radius)
		:
		GeometricObject<T>( x, y ),
		radius(radius)
	{}
	~Circle() {}
	bool IsOverlapWith( const GeometricObject<T>& other ) const override
	{
		if ( const Circle<T>* pCircle = dynamic_cast<const Circle<T>*>(&other) )
		{
			const Vec2<T> distance = GeometricObject<T>::center - pCircle->center;
			const T sumOfRadius = radius + pCircle->radius;
			return distance.GetLength() < sumOfRadius;
		}
		else
		{
			OutputDebugStringW( L"Circle::IsOverlapWith(const GeometricObject<T>& other) const override, other's type mismatch!!!" );
			exit( 1 );
		}
	}
	bool IsContainedBy( const GeometricObject<T>& other ) const override
	{
		if ( const Circle<T>* pCircle = dynamic_cast<const Circle<T>*>(&other) )
		{
			const Vec2<T> distanceSq = GeometricObject<T>::center - pCircle->center;
			const T difference = pCircle->radius - radius;
			return distanceSq.GetLength() < difference;
		}
		else
		{
			OutputDebugStringW( L"Circle::IsContainedBy( const GeometricObject<T>& other ) const override, other's type mismatch!!!" );
			exit( 1 );
		}
	}
	bool IsContains( const GeometricObject<T>& other ) const override
	{
		if ( const Circle<T>* pCircle = dynamic_cast<const Circle<T>*>(&other) )
		{
			const Vec2<T> distance = pCircle->center - GeometricObject<T>::center;
			const T difference = radius - pCircle->radius;
			return distance.GetLength() > difference;
		}
		else
		{
			OutputDebugStringW( L"Circle::IsContains( const GeometricObject<T>& other ) const override, other's type mismatch!!!" );
			exit( 1 );
		}
	}
	bool IsContains( const Vec2<T>& p ) const override
	{
		const Vec2<T> distance = GeometricObject<T>::center - p;
		return distance.GetLengthSq() < radius * radius;
	}
	bool IsContains( T x, T y ) const override
	{
		return IsContains( Vec2<T>{ x, y } );
	}
	double GetArea() const override
	{
		return radius * radius * MathSH::PI;
	}
	double GetPerimeter() const override
	{
		return radius * MathSH::PI * 2;
	}
	void SetRadius( T r )
	{
		radius = r;
	}
	T GetRadius() const
	{
		return radius;
	}
	void Draw(HDC hdc) const override
	{
		const int left = (int)(GeometricObject<T>::center.x - radius);
		const int top = (int)(GeometricObject<T>::center.y - radius);
		const int right = (int)(GeometricObject<T>::center.x + radius);
		const int bottom = (int)(GeometricObject<T>::center.y + radius);

		Ellipse( hdc, left, top, right, bottom );
	}
	void DrawColor( HDC hdc, COLORREF color = 0xFFFFFF ) const
	{
		const int left = (int)(GeometricObject<T>::center.x - radius);
		const int top = (int)(GeometricObject<T>::center.y - radius);
		const int right = (int)(GeometricObject<T>::center.x + radius);
		const int bottom = (int)(GeometricObject<T>::center.y + radius);

		HBRUSH hBrush;
		hBrush = CreateSolidBrush( color );
		SelectObject( hdc, hBrush );
		Ellipse( hdc, left, top, right, bottom );
		DeleteObject( hBrush );
	}
	void DrawSelected( HDC hdc, COLORREF color = 0x0000FF) const
	{
		const int left = (int)(GeometricObject<T>::center.x - radius);
		const int top = (int)(GeometricObject<T>::center.y - radius);
		const int right = (int)(GeometricObject<T>::center.x + radius);
		const int bottom = (int)(GeometricObject<T>::center.y + radius);

		if ( GeometricObject<T>::isSelected )
		{
			Rectangle( hdc, left, top, right, bottom );
			DrawColor( hdc, color );
		}
		else
		{
			Draw( hdc );
		}
	}
	
private:
	T radius;
};

template<typename T>
class Rect : public GeometricObject<T>
{
public:
	Rect()
		:
		GeometricObject<T>( (T)0, (T)0 ),
		width( (T)1 ),
		height( (T)1 )
	{
		SetVertices();
	}
	Rect( const Vec2<T>& center, T width, T height )
		:
		GeometricObject<T>( center ),
		width( width ),
		height( height )
	{
		SetVertices();
	}
	Rect( T x, T y, T width, T height )
		:
		GeometricObject<T>( x, y ),
		width( width ),
		height( height )
	{
		SetVertices();
	}
	~Rect() {}

	bool IsOverlapWith( const GeometricObject<T>& other ) const override
	{
		if ( const Rect<T>* pRect = dynamic_cast<const Rect<T>*>(&other) )
		{
			return right > pRect->left && left < pRect->right&& top > pRect->bottom && bottom < pRect->top;
		}
		else
		{
			OutputDebugStringW( L"Rect::IsOverlapWith( const GeometricObject<T>& other ) const override, other's type mismatch!!!" );
			exit( 1 );
		}
	}
	bool IsContainedBy( const GeometricObject<T>& other ) const override
	{
		if ( const Rect<T>* pRect = dynamic_cast<const Rect<T>*>(&other) )
		{
			return pRect->top >= top && pRect->bottom <= bottom && pRect->left <= left && pRect->right >= right;
		}
		else
		{
			OutputDebugStringW( L"Rect::IsContainedBy( const GeometricObject<T>& other ) const override, other's type mismatch!!!" );
			exit( 1 );
		}
	}
	bool IsContains( const GeometricObject<T>& other ) const override
	{
		if ( const Rect<T>* pRect = dynamic_cast<const Rect<T>*>(&other) )
		{
			return (pRect->top <= top && pRect->bottom >= bottom && pRect->left >= left && pRect->right <= right);
		}
		else
		{
			OutputDebugStringW( L"Rect::IsContains( const GeometricObject<T>& other ) const override, other's type mismatch!!!" );
			exit( 1 );
		}
	}
	bool IsContains( const Vec2<T>& point ) const override
	{
		return point.y <= top && point.y >= bottom && point.x >= left && point.x <= right;
	}
	bool IsContains( T x, T y ) const override
	{
		return IsContains( Vec2<T>{ x, y } );
	}
	double GetArea() const override
	{
		return width * height;
	}
	double GetPerimeter() const override
	{
		return width * 2 + height * 2;
	}
	T GetWidth() const
	{
		return width;
	}
	T GetHeight() const
	{
		return height;
	}
	void SetWidth( T width_in )
	{
		width = width_in;
		SetVertices();
	}
	void SetHeight( T height_in )
	{
		height = height_in;
		SetVertices();
	}
	void SetCenter( const Vec2<T>& center_in ) override
	{
		GeometricObject<T>::center = center_in;
		SetVertices();
	}
	void SetCenter( T x, T y ) override
	{
		SetCenter( { x, y } );
		SetVertices();
	}
	void Draw( HDC hdc ) const override
	{
		Rectangle( hdc, (int)left, (int)top, (int)right, (int)bottom );
	}
	RECT GetRECT() const
	{
		return { left, top, right, bottom };
	}

private:
	void SetVertices()
	{
		const T halfWidth = T( width / 2.0 );
		const T halfHeight = T( height / 2.0 );
		left = GeometricObject<T>::center.x - halfWidth;
		right = GeometricObject<T>::center.x + halfWidth;
		top = GeometricObject<T>::center.y + halfHeight;
		bottom = GeometricObject<T>::center.y - halfHeight;
	}

private:
	T width;
	T height;
	T left;
	T right;
	T top;
	T bottom;
};

template<typename T>
class Star : public GeometricObject<T>
{
public:
	Star( const Vec2<T> center, int outerRadius, int nFlares )
		:
		GeometricObject<T>( center.x, center.y ),
		nFlares( nFlares ),
		outerRadius( outerRadius ),
		dTheta( MathSH::PI / nFlares ),
		innerRadius( (outerRadius* cos( dTheta * 2 )) / (cos( dTheta )) )
	{
		MakeStar();
	}
	Star(const Vec2<T> center, int outerRadius, int innerRadius, int nFlares )
		:
		GeometricObject<T>( center.x, center.y ),
		nFlares( nFlares ),
		outerRadius(outerRadius),
		innerRadius(innerRadius),
		dTheta( MathSH::PI / nFlares )
	{
		MakeStar();
	}

	void Draw( HDC hdc ) const override
	{
		const int size = nFlares * 2;
		std::vector<POINT> points;
		points.reserve( size );
		for ( auto e : starPoints )
		{
			const POINT p = { e.x, e.y };
			points.push_back( p );
		}

		Polygon( hdc, &points[0], size );
	}
	
private:
	void MakeStar()
	{
		starPoints.reserve( nFlares * 2 );
		for ( int i = 0; i < nFlares * 2; i++ )
		{
			const double rad = (i % 2 == 0) ? outerRadius : innerRadius;
			starPoints.emplace_back( (T)(GeometricObject<T>::center.x + rad * cos( i * dTheta )),
				(T)(GeometricObject<T>::center.y + rad * sin( i * dTheta )) );
		}
	}

private:
	const int nFlares;
	const double dTheta;
	int outerRadius;
	int innerRadius;
	std::vector<Vec2<T>> starPoints;
};

template<typename T>
class Point : public GeometricObject<T>
{
public:
	Point() {}
	Point( T x, T y )
		:
		GeometricObject<T>( x, y )
	{}
	~Point() {}

	Point<T> operator+( const Point<T>& rhs ) const
	{
		return { GeometricObject<T>::x + rhs.x , GeometricObject<T>::y + rhs.y };
	}
	Point<T>& operator+=( const Point<T>& rhs ) const
	{
		return *this = *this + rhs;
	}
	Point<T> operator-( const Point<T>& rhs ) const
	{
		return { GeometricObject<T>::x - rhs.x , GeometricObject<T>::y - rhs.y };
	}
	Point<T>& operator-=( const Point<T>& rhs ) const
	{
		return *this = *this - rhs;
	}
	T GetLength() const
	{
		return std::sqrt( GetLengthSq() );
	}
	T GetLengthSq() const
	{
		return GeometricObject<T>::x * GeometricObject<T>::x + GeometricObject<T>::y * GeometricObject<T>::y;
	}

	void Draw( HDC hdc ) const override
	{
		SetPixel( hdc, GeometricObject<T>::center.x, GeometricObject<T>::center.y, RGB( 0, 0, 0 ) );
	}

	static bool IsContains( const GeometricObject<T>& other, const GeometricObject<T>& p )
	{
		if ( const Point<T>* pPoint = dynamic_cast<const Point<T>*>(&p) )
		{
			if ( const Rect<T>* pRect = dynamic_cast<const Rect<T>*>(&other) )
			{
				return pRect->IsContains( pPoint->GetCenterX(), pPoint->GetCenterY() );
			}
			else if ( const Circle<T>* pCircle = dynamic_cast<const Circle<T>*>(&other) )
			{
				return pCircle->IsContains( pPoint->GetCenterX(), pPoint->GetCenterY() );
			}
			else
			{
				OutputDebugStringW( L"Point::IsContains( const GeometricObject<T>& other, const GeometricObject<T>& p ), other's type mismatch" );
				exit( 1 );
			}
		}
		else
		{
			OutputDebugStringW( L"Point::IsContains( const GeometricObject<T>& other, const GeometricObject<T>& p ), p's type mismatch" );
			exit( 1 );
		}
	}
};