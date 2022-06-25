#pragma once

#include "Vec2.h"
#include "Mat3.h"
#include "MathSH.h"
#include "framework.h"
#include <vector>

template<typename T>
class Rect;
template<typename T>
class Circle;

template<typename T>
class GeometricObject
{
public:
	GeometricObject()
		:
		center( (T)0, (T)0 )
	{
	}
	GeometricObject( const Vec2<T> center )
		:
		center( center )
	{
	}
	GeometricObject( T x, T y )
		:
		center( x, y )
	{
	}
	virtual ~GeometricObject() {}

	virtual bool IsOverlapWith( const GeometricObject<T>& ) const = 0;
	virtual bool IsContainedBy( const GeometricObject<T>& ) const = 0;
	virtual bool IsContains( const GeometricObject<T>& ) const { return false; }
	virtual bool IsContains( const Vec2<T>& ) const { return false; }
	virtual bool IsContains( T, T ) const { return false; }
	virtual double GetArea() const { return 0; }
	virtual double GetPerimeter() const { return 0; }
	virtual T GetRadius() const { return 0; }
	virtual RECT GetRECT() const { return { 0,0,0,0 }; }
	virtual T GetSize() const = 0;
	virtual void Draw( HDC hdc ) const = 0;
	virtual void DrawTransformed( HDC hdc, const Mat3<T>& transform_in ) const { return; }
	virtual void DrawDebug( HDC hdc ) const = 0;
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
	void SetCenterX(T x)
	{
		center.x = x;
	}
	void SetCenterY( T y )
	{
		center.y = y;
	}
	std::vector<Vec2<T>> GetVertices() const
	{
		return vertices;
	}
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
	void TransformVertices( const Mat3<T>& transform )
	{
		for ( auto& v : vertices )
		{
			v = transform * v;
		}
	}
	void ApplyTransformation(const Mat3<T>& transform_in )
	{
		transform =  transform_in;
	}

	bool CheckVerticesSAT( const GeometricObject<T>& shape1, const GeometricObject<T>& shape2 ) const
	{
		for ( int vIdx = 0; vIdx < shape1.vertices.size(); ++vIdx )
		{
			const int vIdxNext = (vIdx + 1) % shape1.vertices.size();
			Vec2<T> axisProj = (shape1.vertices[vIdx] - shape1.vertices[vIdxNext]).GetNormalLeftVec2();

			float minThis = INFINITY;
			float maxThis = -INFINITY;
			for ( auto e : shape1.vertices )
			{
				const float p = e * axisProj;
				minThis = (std::min)(minThis, p);
				maxThis = (std::max)(maxThis, p);
			}

			float minOther = INFINITY;
			float maxOther = -INFINITY;
			for ( auto e : shape2.vertices )
			{
				const float p = e * axisProj;
				minOther = (std::min)(minOther, p);
				maxOther = (std::max)(maxOther, p);
			}

			if ( !(maxOther >= minThis && maxThis >= minOther) )
			{
				return false;
			}
		}
		return true;
	}

	bool CheckCircleOverlap(const Circle<T>& c1, const Circle<T>& c2) const
	{
		const Vec2<T> distance = c1.center - c2.center;
		const T sumOfRadius = c1.GetRadius() + c2.GetRadius();
		return (T)abs( distance.x * distance.x + distance.y * distance.y ) < sumOfRadius * sumOfRadius;
	}

	bool CheckConvexOverlapWithConvex( const GeometricObject<T>& other ) const
	{
		Circle<T> thisOuterCircle( center, GetRadius() );
		Circle<T> otherOuterCircle( other.center, other.GetRadius() );

		if ( CheckCircleOverlap( thisOuterCircle, otherOuterCircle ) )
		{
			if ( CheckVerticesSAT( *this, other ) == false )
			{
				return false;
			}
			if ( CheckVerticesSAT( other, *this ) == false )
			{
				return false;
			}
			return true;
		}
		return false;
	}

	bool CheckConvexOverlapWithLine( const Vec2<T>& line ) const
	{
		for ( int vIdx = 0; vIdx < vertices.size(); ++vIdx )
		{
			const int vIdxNext = (vIdx + 1) % vertices.size();
			Vec2<T> axisProj = (vertices[vIdx] - vertices[vIdxNext]).GetNormalLeftVec2();

			float minThis = INFINITY;
			float maxThis = -INFINITY;
			for ( auto e : vertices )
			{
				const float p = e * axisProj;
				minThis = (std::min)(minThis, p);
				maxThis = (std::max)(maxThis, p);
			}

			float minOther = INFINITY;
			float maxOther = -INFINITY;
			const float p = line * axisProj;
			minOther = (std::min)(minOther, p);
			maxOther = (std::max)(maxOther, p);

			if ( !(maxOther >= minThis && maxThis >= minOther) )
			{
				return false;
			}
		}
		return true;
	}

	bool CheckConvexOverlapWithCircle( const GeometricObject<T>& convex, const GeometricObject<T>& circle ) const
	{
		for ( int vIdx = 0; vIdx < convex.vertices.size(); ++vIdx )
		{
			const int vIdxNext = (vIdx + 1) % convex.vertices.size();
			Vec2<T> axisProj = (convex.vertices[vIdx] - convex.vertices[vIdxNext]).GetNormalLeftVec2();

			float minThis = INFINITY;
			float maxThis = -INFINITY;
			for ( auto e : convex.vertices )
			{
				const float p = e * axisProj;
				minThis = (std::min)(minThis, p);
				maxThis = (std::max)(maxThis, p);
			}

			float minOther = INFINITY;
			float maxOther = -INFINITY;

			const Vec2<T> normalizedAxis = axisProj.GetNormalized();
			float p = (circle.vertices[0] + (normalizedAxis * circle.GetSize())) * axisProj;
			minOther = (std::min)(minOther, p);
			maxOther = (std::max)(maxOther, p);
			p = (circle.vertices[0] - (normalizedAxis * circle.GetSize())) * axisProj;
			minOther = (std::min)(minOther, p);
			maxOther = (std::max)(maxOther, p);

			if ( !(maxOther >= minThis && maxThis >= minOther) )
			{
				return false;
			}
		}
		return true;
	}

protected:
	Vec2<T> center;
	std::vector<Vec2<T>> vertices;
	Mat3<T> transform = Mat3<T>::Identity();
	COLORREF color = 0xFFFFFF;
	bool isSelected = false;
};

template<typename T>
class Circle : public GeometricObject<T>
{
public:
	Circle()
	{
		GeometricObject<T>::vertices.push_back( GeometricObject<T>::center );
	}
	Circle(const Vec2<T>& center, T radius )
		:
		GeometricObject<T>(center),
		radius(radius)
	{
		GeometricObject<T>::vertices.push_back( GeometricObject<T>::center );
	}
	Circle(T x, T y, T radius)
		:
		GeometricObject<T>( x, y ),
		radius(radius)
	{
		GeometricObject<T>::vertices.push_back( GeometricObject<T>::center );
	}
	~Circle() {}
	bool IsOverlapWith( const GeometricObject<T>& other ) const override
	{
		if ( const Circle<T>* pCircle = dynamic_cast<const Circle<T>*>(&other) )
		{
			return this->IsOverlapWith( *pCircle );
		}
		else if ( const Rect<T>* pRect = dynamic_cast<const Rect<T>*>(&other) )
		{
			return this->CheckConvexOverlapWithCircle( other, *this );
		}
		return false;
	}
	bool IsOverlapWith( const Circle<T>& other ) const
	{
		const Vec2<T> distance = GeometricObject<T>::center - other.center;
		const T sumOfRadius = radius + other.radius;
		return (T)abs( distance.x * distance.x + distance.y * distance.y ) < sumOfRadius * sumOfRadius;
	}
	bool IsContainedBy( const GeometricObject<T>& other ) const override
	{
		if ( const Circle<T>* pCircle = dynamic_cast<const Circle<T>*>(&other) )
		{
			const Vec2<T> distanceSq = GeometricObject<T>::center - pCircle->center;
			const T difference = pCircle->radius - radius;
			return distanceSq.GetLength() < difference;
		}
		return false;
	}
	bool IsContains( const GeometricObject<T>& other ) const override
	{
		if ( const Circle<T>* pCircle = dynamic_cast<const Circle<T>*>(&other) )
		{
			const Vec2<T> distance = pCircle->center - GeometricObject<T>::center;
			const T difference = radius - pCircle->radius;
			return distance.GetLength() > difference;
		}
		return false;
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
	void SetCenter( const Vec2<T>& p ) override
	{
		GeometricObject<T>::center = p;
		GeometricObject<T>::vertices[0] = GeometricObject<T>::center;
	}
	void SetCenter( T x, T y ) override
	{
		GeometricObject<T>::center = { x, y };
		GeometricObject<T>::vertices[0] = GeometricObject<T>::center;
	}
	void SetRadius( T r )
	{
		radius = r;
	}
	T GetRadius() const override
	{
		return radius;
	}
	T GetSize() const override
	{
		return radius;
	}
	RECT GetRECT() const override
	{
		const int left = (int)(GeometricObject<T>::center.x - radius);
		const int top = (int)(GeometricObject<T>::center.y - radius);
		const int right = (int)(GeometricObject<T>::center.x + radius);
		const int bottom = (int)(GeometricObject<T>::center.y + radius);
		return { left, top, right, bottom };
	}
	void Draw(HDC hdc) const override
	{
		const int left = (int)(GeometricObject<T>::center.x - radius);
		const int top = (int)(GeometricObject<T>::center.y - radius);
		const int right = (int)(GeometricObject<T>::center.x + radius);
		const int bottom = (int)(GeometricObject<T>::center.y + radius);

		Ellipse( hdc, left, top, right, bottom );
	}
	void DrawTransformed( HDC hdc, const Mat3<T>& transform_in ) const override
	{
		Draw( hdc );
	}
	void DrawDebug( HDC hdc ) const override
	{
		DrawColor( hdc, RGB( 255,0,0 ) );
	}
	void DrawColor( HDC hdc, COLORREF color = 0xFFFFFF ) const
	{
		const int left = (int)(GeometricObject<T>::center.x - radius);
		const int top = (int)(GeometricObject<T>::center.y - radius);
		const int right = (int)(GeometricObject<T>::center.x + radius);
		const int bottom = (int)(GeometricObject<T>::center.y + radius);

		HBRUSH hBrush;
		HBRUSH oldBrush;
		hBrush = CreateSolidBrush( color );
		oldBrush = (HBRUSH)SelectObject( hdc, hBrush );
		Ellipse( hdc, left, top, right, bottom );
		SelectObject( hdc, oldBrush );
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
		const T halfWidth = (T)0.5 * width;
		outerRadius = (T)std::sqrt( halfWidth * halfWidth + halfWidth * halfWidth );
		GeometricObject<T>::vertices.resize( 4 );
		SetVertices();
	}
	Rect( const Vec2<T>& center, T width, T height )
		:
		GeometricObject<T>( center ),
		width( width ),
		height( height )
	{
		const T halfWidth = (T)0.5 * width;
		outerRadius = (T)std::sqrt( halfWidth * halfWidth + halfWidth * halfWidth );
		GeometricObject<T>::vertices.resize( 4 );
		SetVertices();
	}
	Rect( T x, T y, T width, T height )
		:
		GeometricObject<T>( x, y ),
		width( width ),
		height( height )
	{
		const T halfWidth = (T)0.5 * width;
		outerRadius = (T)std::sqrt( halfWidth * halfWidth + halfWidth * halfWidth );
		GeometricObject<T>::vertices.resize( 4 );
		SetVertices();
	}
	Rect( const Vec2<T>& topLeft, const Vec2<T>& bottomRight )
		:
		GeometricObject<T>( (topLeft + bottomRight)* (T)0.5 ),
		width( bottomRight.x - topLeft.x ),
		height( bottomRight.y - topLeft.y )
	{
		const T halfWidth = (T)0.5 * width;
		outerRadius = (T)std::sqrt( halfWidth * halfWidth + halfWidth * halfWidth );
		GeometricObject<T>::vertices.resize( 4 );
		SetVertices();
	}

	~Rect() {}


	bool IsOverlapWith( const GeometricObject<T>& other ) const override
	{
		if ( const Rect<T>* pRect = dynamic_cast<const Rect<T>*>(&other) )
		{
			return this->CheckConvexOverlapWithConvex( other );
		}
		else if ( const Circle<T>* pCircle = dynamic_cast<const Circle<T>*>(&other) )
		{
			return this->CheckConvexOverlapWithCircle( *this, other );
		}
		return false;
	}
	bool IsContainedBy( const GeometricObject<T>& other ) const override
	{
		if ( const Rect<T>* pRect = dynamic_cast<const Rect<T>*>(&other) )
		{
			return pRect->top >= top && pRect->bottom <= bottom && pRect->left <= left && pRect->right >= right;
		}
		return false;
	}
	bool IsContains( const GeometricObject<T>& other ) const override
	{
		if ( const Rect<T>* pRect = dynamic_cast<const Rect<T>*>(&other) )
		{
			return (pRect->top <= top && pRect->bottom >= bottom && pRect->left >= left && pRect->right <= right);
		}
		return false;
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
	T GetRadius() const override
	{
		return outerRadius;
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
	T GetSize() const override
	{
		return width;
	}

	void Draw( HDC hdc ) const override
	{
		std::vector<POINT> points;
		for ( auto e : GeometricObject<T>::vertices )
		{
			points.push_back( {(LONG)e.x, (LONG)e.y} );
		}
		Polygon( hdc, &points[0], points.size() );
	}

	void DrawTransformed( HDC hdc, const Mat3<T>& transform_in ) const override
	{
		const Vec2<T> topLeftVec = transform_in * (GeometricObject<T>::center - Vec2<T>{ (T)left, (T)top }) + GeometricObject<T>::center;
		const Vec2<T> topRightVec = transform_in * (GeometricObject<T>::center - Vec2<T>{ (T)right, (T)top }) + GeometricObject<T>::center;
		const Vec2<T> bottomRightVec = transform_in * (GeometricObject<T>::center - Vec2<T> { (T)right, (T)bottom }) + GeometricObject<T>::center;
		const Vec2<T> bottomLeftVec = transform_in * (GeometricObject<T>::center - Vec2<T>{ (T)left, (T)bottom }) + GeometricObject<T>::center;

		const POINT topLeft = { (int)topLeftVec.x, (int)topLeftVec.y };
		const POINT topRight = { (int)topRightVec.x, (int)topRightVec.y };
		const POINT bottomRight = { (int)bottomRightVec.x, (int)bottomRightVec.y };
		const POINT bottomLeft = { (int)bottomLeftVec.x, (int)bottomLeftVec.y };

		const std::vector<POINT> vertices = { topLeft, topRight, bottomRight, bottomLeft };

		Polygon( hdc, &vertices[0], vertices.size() );
	}
	void DrawDebug( HDC hdc ) const override
	{
		std::vector<POINT> points;
		for ( auto e : GeometricObject<T>::vertices )
		{
			points.push_back( { (LONG)e.x, (LONG)e.y } );
		}

		HBRUSH hBrush;
		HBRUSH oldBrush;
		hBrush = CreateSolidBrush( 0x0000FF );
		oldBrush = (HBRUSH)SelectObject( hdc, hBrush );
		Polygon( hdc, &points[0], points.size() );
		SelectObject( hdc, oldBrush );
		DeleteObject( hBrush );

	}
	RECT GetRECT() const override
	{
		return { (int)left, (int)top, (int)right, (int)bottom };
	}

private:
	void SetVertices()
	{
		const T halfWidth = T( width / 2.0 );
		const T halfHeight = T( height / 2.0 );
		left = GeometricObject<T>::center.x - halfWidth;
		right = GeometricObject<T>::center.x + halfWidth;
		top = GeometricObject<T>::center.y - halfHeight;
		bottom = GeometricObject<T>::center.y + halfHeight;

		GeometricObject<T>::vertices[0] = GeometricObject<T>::transform * (GeometricObject<T>::center - Vec2<T>{ (T)left, (T)top }) + GeometricObject<T>::center;
		GeometricObject<T>::vertices[1] = GeometricObject<T>::transform * (GeometricObject<T>::center - Vec2<T>{ (T)right, (T)top }) + GeometricObject<T>::center;
		GeometricObject<T>::vertices[2] = GeometricObject<T>::transform * (GeometricObject<T>::center - Vec2<T> { (T)right, (T)bottom }) + GeometricObject<T>::center;
		GeometricObject<T>::vertices[3] = GeometricObject<T>::transform * (GeometricObject<T>::center - Vec2<T>{ (T)left, (T)bottom }) + GeometricObject<T>::center;
	}

private:
	T width;
	T height;
	T left;
	T right;
	T top;
	T bottom;
	T outerRadius;
};

template<typename T>
class Star : public GeometricObject<T>
{
public:
	Star( const Vec2<T> center, T outerRadius, int nFlares )
		:
		GeometricObject<T>( center.x, center.y ),
		nFlares( nFlares ),
		outerRadius( outerRadius ),
		dTheta( MathSH::PI / nFlares ),
		innerRadius( (T)((outerRadius* cos( dTheta * 2 )) / (cos( dTheta ))) )
	{
		GeometricObject<T>::vertices.resize( nFlares * 2 );
		SetVertices();
	}
	Star(const Vec2<T> center, T outerRadius, T innerRadius, int nFlares )
		:
		GeometricObject<T>( center.x, center.y ),
		nFlares( nFlares ),
		outerRadius(outerRadius),
		innerRadius(innerRadius),
		dTheta( MathSH::PI / nFlares )
	{
		GeometricObject<T>::vertices.resize( nFlares * 2 );
		SetVertices();
	}

	bool IsOverlapWith( const GeometricObject<T>& other ) const override
	{
		return false;
	}

	bool IsContainedBy( const GeometricObject<T>& other ) const override
	{
		if ( const Circle<T>* pCircle = dynamic_cast<const Circle<T>*>(&other) )
		{
			const Vec2<T> distanceSq = GeometricObject<T>::center - pCircle->GetCenter();
			const T difference = pCircle->GetRadius() - outerRadius;
			return distanceSq.GetLength() < difference;
		}
		return false;
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
		const int size = GeometricObject<T>::vertices.size();
		std::vector<POINT> points;
		points.reserve( size );

		for ( auto e : GeometricObject<T>::vertices )
		{
			const POINT p = { (int)e.x, (int)e.y };
			points.push_back( p );
		}

		Polygon( hdc, &points[0], size );
	}

	void DrawDebug( HDC hdc ) const override
	{
		const int size = GeometricObject<T>::vertices.size();
		std::vector<POINT> points;
		points.reserve( size );

		for ( auto e : GeometricObject<T>::vertices )
		{
			const POINT p = { (int)e.x, (int)e.y };
			points.push_back( p );
		}

		HBRUSH hBrush;
		HBRUSH oldBrush;
		hBrush = CreateSolidBrush( 0x0000FF );
		SelectObject( hdc, hBrush );
		oldBrush = (HBRUSH)SelectObject( hdc, hBrush );
		Polygon( hdc, &points[0], size );
		SelectObject( hdc, oldBrush );
		DeleteObject( hBrush );
	}

	RECT GetRECT() const override
	{
		const int left = (int)(GeometricObject<T>::center.x - outerRadius);
		const int top = (int)(GeometricObject<T>::center.y - outerRadius);
		const int right = (int)(GeometricObject<T>::center.x + outerRadius);
		const int bottom = (int)(GeometricObject<T>::center.y + outerRadius);
		return { left, top, right, bottom };
	}
	
	T GetSize() const override
	{
		return outerRadius;
	}
private:
	void SetVertices()
	{
		for ( int i = 0; i < nFlares * 2; ++i )
		{
			const double rad = (i % 2 == 0) ? outerRadius : innerRadius;
			const Vec2<T> tranformedVec = GeometricObject<T>::transform * Vec2<T>( (T)(rad* cos( i* dTheta )), (T)(rad* sin( i* dTheta )) );
			GeometricObject<T>::vertices[i] = tranformedVec + GeometricObject<T>::center;
		}
	}

private:
	const int nFlares;
	const double dTheta;
	T outerRadius;
	T innerRadius;
};