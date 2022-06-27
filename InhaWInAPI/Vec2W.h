#pragma once

#include "Vec2.h"

template <typename T>
class Vec2W
{
public:
	Vec2W()
		:
		x( T( 0 ) ),
		y( T( 0 ) )
	{}
	Vec2W( T x, T y )
		:
		x( x ),
		y( y )
	{}
	Vec2W( const Vec2W& vect )
		:
		Vec2W( vect.x, vect.y )
	{}
	explicit Vec2W( const Vec2<T>& v2 )
		:
		Vec2W( v2.x, v2.y )
	{
	}
	explicit operator Vec2<T>() const
	{
		return { x,y };
	}
	template <typename T2>
	explicit operator Vec2W<T2>() const
	{
		return{ (T2)x,(T2)y };
	}
	T		LenSq() const
	{
		// return sq( *this );
		return x * x + y * y;
	}
	T		Len() const
	{
		return sqrt( LenSq() );
	}
	Vec2W& Normalize()
	{
		const T length = Len();
		x /= length;
		y /= length;
		return *this;
	}
	Vec2W	GetNormalized() const
	{
		Vec2W norm = *this;
		norm.Normalize();
		return norm;
	}
	Vec2W& Rotate( T angle )
	{
		const T sinT = sin( angle );
		const T cosT = cos( angle );
		const T new_x = x * cosT - y * sinT;
		y = x * sinT + y * cosT;
		x = new_x;

		return *this;
	}
	Vec2W	GetRotated( T angle ) const
	{
		return Vec2W( *this ).Rotate( angle );
	}
	Vec2W	operator-() const
	{
		return Vec2W( -x, -y );
	}
	Vec2W& operator=( const Vec2W& rhs )
	{
		x = rhs.x;
		y = rhs.y;
		w = rhs.w;
		return *this;
	}
	Vec2W& operator+=( const Vec2W& rhs )
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Vec2W& operator-=( const Vec2W& rhs )
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	Vec2W	operator+( const Vec2W& rhs ) const
	{
		return Vec2W( *this ) += rhs;
	}
	Vec2W	operator-( const Vec2W& rhs ) const
	{
		return Vec2W( *this ) -= rhs;
	}
	Vec2W& operator*=( const T& rhs )
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}
	Vec2W	operator*( const T& rhs ) const
	{
		return Vec2W( *this ) *= rhs;
	}
	T	operator*( const Vec2W& rhs ) const
	{
		return x * rhs.x + y * rhs.y;
	}
	Vec2W& operator/=( const T& rhs )
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}
	Vec2W	operator/( const T& rhs ) const
	{
		return Vec2W( *this ) /= rhs;
	}
	bool	operator==( const Vec2W& rhs ) const
	{
		return x == rhs.x && y == rhs.y;
	}
	bool	operator!=( const Vec2W& rhs ) const
	{
		return !(*this == rhs);
	}
public:
	T x;
	T y;
	T w = (T)1;
};