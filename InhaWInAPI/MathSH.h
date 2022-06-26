#pragma once

#include <limits>
#include <cmath>

class MathSH
{
public:
	template <typename T>
	static bool Compare(T lhs, T rhs)
	{
		return std::fabs( lhs - rhs ) < std::numeric_limits<T>::epsilon();
	}

	template <typename T>
	inline static T WrapAngle( T theta )
	{
		const T modded = fmod( theta, (T)2.0 * (T)PI );
		return (modded > (T)PI) ? (modded - (T)2.0 * (T)PI) : modded;
	}

private:
public:
	static constexpr double PI = 3.14159265358979;
};