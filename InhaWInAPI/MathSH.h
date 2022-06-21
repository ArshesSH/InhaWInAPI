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
private:
public:
	static constexpr double PI = 3.14159265358979;
};