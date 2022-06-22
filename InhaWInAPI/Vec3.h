#pragma once

#include "Vec2.h"

template <typename T>
class Vec3 : public Vec2<T>
{
public:
	Vec3() { z = 0; }
	Vec3( T x, T y, T z )
		:
		Vec2<T>( x, y ),
		z( z )
	{}
	Vec3( const Vec3& vect )
		:
		Vec3( vect.x, vect.y, vect.z )
	{}
	template <typename T2>
	explicit operator Vec3<T2>() const
	{
		return{ (T2)this->x,(T2)this->y,(T2)this->z };
	}
	T		LenSq() const
	{
		return this->x* this->x + this->y* this->y + this->z* this->z;
	}
	T		Len() const
	{
		return std::sqrt( LenSq() );
	}
	Vec3& Normalize()
	{
		const T length = Len();
		this->x /= length;
		this->y /= length;
		this->z /= length;
		return *this;
	}
	Vec3	GetNormalized() const
	{
		Vec3 norm = *this;
		norm.Normalize();
		return norm;
	}
	Vec3	operator-() const
	{
		return Vec3( -this->x, -this->y, -this->z );
	}
	Vec3& operator=( const Vec3& rhs )
	{
		this->x = rhs.x;
		this->y = rhs.y;
		this->z = rhs.z;
		return *this;
	}
	Vec3& operator+=( const Vec3& rhs )
	{
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		return *this;
	}
	Vec3& operator-=( const Vec3& rhs )
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		return *this;
	}
	T		operator*( const Vec3& rhs ) const
	{
		return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z;
	}
	Vec3	operator+( const Vec3& rhs ) const
	{
		return Vec3( *this ) += rhs;
	}
	Vec3	operator-( const Vec3& rhs ) const
	{
		return Vec3( *this ) -= rhs;
	}
	Vec3& operator*=( const T& rhs )
	{
		this->x *= rhs;
		this->y *= rhs;
		this->z *= rhs;
		return *this;
	}
	Vec3	operator*( const T& rhs ) const
	{
		return Vec3( *this ) *= rhs;
	}
	Vec3& operator/=( const T& rhs )
	{
		this->x /= rhs;
		this->y /= rhs;
		this->z /= rhs;
		return *this;
	}
	Vec3	operator/( const T& rhs ) const
	{
		return Vec3( *this ) /= rhs;
	}
	bool	operator==( const Vec3& rhs ) const
	{
		return this->x == rhs.x && this->y == rhs.y && rhs.z = this->z;
	}
	bool	operator!=( const Vec3& rhs ) const
	{
		return !(*this == rhs);
	}
	static Vec3 GetCrossProduct( const Vec3& lhs, const Vec3& rhs )
	{
		return
		{
			Vec3(
				lhs.y * rhs.z - lhs.z * rhs.y,
				lhs.z * rhs.x - lhs.x * rhs.z,
				lhs.x * rhs.y - lhs.y * rhs.x
			)
		};
	}
public:
	T z;
};