#pragma once

#include <cmath>

template<typename T>
class Vec2
{
public:
	Vec2() { x = 0, y = 0; }
	Vec2( T x_in, T y_in )
		:
		x( x_in ),
		y( y_in )
	{
	}

	template<typename S>
	explicit Vec2( const Vec2<S>& src )
		:
		x( (T)src.x ),
		y( (T)src.y )
	{
	}
	Vec2& operator=( const Vec2& rhs )
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
	Vec2 operator+( const Vec2& rhs ) const
	{
		return Vec2( x + rhs.x, y + rhs.y );
	}

	Vec2& operator+=( const Vec2& rhs )
	{
		return *this = *this + rhs;
	}

	Vec2 operator*( T rhs ) const
	{
		return Vec2( x * rhs, y * rhs );
	}

	Vec2& operator*=( T rhs )
	{
		return *this = *this * rhs;
	}

	T operator*( Vec2<T> rhs ) const
	{
		return T( x * rhs.x + y * rhs.y );
	}

	Vec2 operator-( const Vec2& rhs ) const
	{
		return Vec2( x - rhs.x, y - rhs.y );
	}

	Vec2& operator-=( const Vec2& rhs )
	{
		return *this = *this - rhs;
	}
	Vec2 operator/( T rhs ) const
	{
		return Vec2( x / rhs, y / rhs );
	}
	Vec2& operator/=( T rhs )
	{
		return *this = *this / rhs;
	}
	bool operator==( const Vec2& rhs ) const
	{
		return x == rhs.x && y == rhs.y;
	}

	T GetLength() const
	{
		return std::sqrt( GetLengthSq() );
	}

	T GetLengthSq() const
	{
		return x * x + y * y;
	}

	Vec2& Normalize()
	{
		return *this = GetNormalized();
	}

	Vec2 GetNormalized() const
	{
		const T len = GetLength();
		if ( len != (T)0 )
		{
			return *this * ((T)1 / len);
		}
		return *this;
	}

	Vec2 GetNormalRightVec2() const
	{
		return { y, -x };
	}
	Vec2 GetNormalLeftVec2() const
	{
		return { -y, x };
	}

	static T GetCrossProduct( const Vec2& lhs, const Vec2& rhs )
	{
		return T(lhs.x * rhs.y - lhs.y * rhs.x);
	}

public:
	T x;
	T y;
};