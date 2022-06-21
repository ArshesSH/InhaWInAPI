#pragma once

#include "Vec2.h"
#include "MathSH.h"

template<typename T>
class Circle2D
{
public:
	Circle2D()
		:
		center( 0, 0 ),
		radius( 1 )
	{}
	Circle2D( const Vec2<T>& center, T radius )
		:
		center( center ),
		radius( radius )
	{}
	Circle2D( T x, T y, T radius )
		:
		center( { x, y } ),
		radius( radius )
	{
	}
	bool IsOverlapWith( const Circle2D<T>& other ) const
	{
		const Vec2<T> distance = center - other.center;
		const T sumOfRadius = radius + other.radius;
		return distance.GetLength() < sumOfRadius ;
	}
	bool IsContainedBy( const Circle2D<T>& other ) const
	{
		const Vec2<T> distanceSq = center - other.center;
		const T difference = other.radius - radius;
		return distanceSq.GetLength() < difference;
	}
	bool IsContains( const Circle2D<T>& other ) const
	{
		const Vec2<T> distance = other.center - center;
		const T difference = radius - other.radius;
		return distance.GetLength() > difference;
	}
	bool IsContains( const Vec2<T>& p ) const
	{
		const Vec2<T> distance = center - p;
		return distance.GetLengthSq() < radius * radius;
	}
	bool IsContains( T x, T y ) const
	{
		return IsContains( { x, y } );
	}
	double GetArea() const
	{
		return radius * radius * MathSH::PI;
	}
	double GetPerimeter() const
	{
		return radius * MathSH::PI * 2;
	}
	void SetCenter(const Vec2<T>& p)
	{
		center = p;
	}
	void SetCenter( T x, T y )
	{
		SetCenter( { x, y } );
	}
	void SetRadius( T r )
	{
		radius = r;
	}
	Vec2<T> GetCenter() const
	{
		return center;
	}
	T GetRadius() const
	{
		return radius;
	}

private:
	Vec2<T> center;
	T radius;
};