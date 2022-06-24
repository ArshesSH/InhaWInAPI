#pragma once

#include "Vec2W.h"

template<typename T>
class Mat3
{
public:
	Vec2<T> operator*( const Vec2<T>& v )const
	{
		return Vec2<T>( *this * (Vec2W<T>)v );
	}
	Vec2W<T> operator*( const Vec2W<T>& v ) const
	{
		Vec2W<T> vout;
		vout.x = cells[0][0] * v.x + cells[0][1] * v.y + cells[0][2] * v.w;
		vout.y = cells[1][0] * v.x + cells[1][1] * v.y + cells[1][2] * v.w;
		vout.w = cells[2][0] * v.x + cells[2][1] * v.y + cells[2][2] * v.w;
		return vout;
	}
	Mat3 operator*( const Mat3& rhs ) const
	{
		Mat3 out;
		for ( int row = 0; row < 3; row++ )
		{
			for ( int col = 0; col < 3; col++ )
			{
				out.cells[row][col] = (T)0;
				for ( int i = 0; i < 3; i++ )
				{
					out.cells[row][col] += cells[row][i] * rhs.cells[i][col];
				}
			}
		}
		return out;
	}
	Mat3& operator*=( const Mat3& rhs ) const
	{
		return *this = *this * rhs;
	}
	static Mat3 Scale( T factor )
	{
		return {
			factor,	(T)0,	(T)0,
			(T)0,	factor,	(T)0,
			(T)0,	(T)0,	(T)1
		};
	}
	static Mat3 ScaleIndependent( T x, T y )
	{
		return {
			x,	(T)0,	(T)0,
			(T)0,	y,	(T)0,
			(T)0,	(T)0,	(T)1
		};
	}
	static Mat3 Identity()
	{
		return Scale( (T)1 );
	}
	static Mat3 FlipY()
	{
		return {
			(T)1,	(T)0,	(T)0,
			(T)0,	(T)-1,	(T)0,
			(T)0,	(T)0,	(T)1
		};
	}
	static Mat3 Rotation( T theta )
	{
		const auto cosT = std::cos( theta );
		const auto sinT = std::sin( theta );
		return{
			cosT,	-sinT,	(T)0,
			sinT,	cosT,	(T)0,
			(T)0,	(T)0,	(T)1
		};
	}
	static Mat3 Translation( T x, T y )
	{
		return{
			(T)1,	(T)0,	(T)x,
			(T)0,	(T)1,	(T)y,
			(T)0,	(T)0,	(T)1
		};
	}
	static Mat3 Translation( const Vec2<T>& v )
	{
		return{
			(T)1,	(T)0,	(T)v.x,
			(T)0,	(T)1,	(T)v.y,
			(T)0,	(T)0,	(T)1
		};
	}
public:
	// [row][col]
	T cells[3][3];
};
