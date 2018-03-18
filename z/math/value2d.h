#pragma once
#ifndef VALUE2D_H_INCLUDED
#define VALUE2D_H_INCLUDED

namespace z
{
	namespace math
	{
		template <typename T>
		struct value2d
		{
			T x, y;

			value2d() : x(0), y(0) {}
			value2d(const T& _x, const T& _y) : x(_x), y(_y) {}
			value2d(const value2d& other) : x(other.x), y(other.y) {}
		};
	}
}

#endif //VALUE2D_H_INCLUDED