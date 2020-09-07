#pragma once

#include <cstring>
#include "typeChecks.hpp"

namespace z
{
	namespace core
	{
		template <typename T>
		static typename std::enable_if<types::equalExists<T>::value, bool>::type
		equals(const T& arg1, const T& arg2) noexcept
		{
			return arg1 == arg2;
		}

		template <typename T>
		static typename std::enable_if<!types::equalExists<T>::value, bool>::type
		equals(const T& arg1, const T& arg2) noexcept
		{
			return !std::memcmp(&arg1, &arg2, sizeof(T));
		}

		template <typename T>
		static typename std::enable_if<types::greaterExists<T>::value, bool>::type
		greater(const T& arg1, const T& arg2) noexcept
		{
			return arg1 > arg2;
		}

		template <typename T>
		static typename std::enable_if<!types::greaterExists<T>::value, bool>::type
		greater(const T& arg1, const T& arg2) noexcept
		{
			return std::memcmp(&arg1, &arg2, sizeof(T)) > 0;
		}

		template <typename T>
		static typename std::enable_if<types::lesserExists<T>::value, bool>::type
		lesser(const T& arg1, const T& arg2) noexcept
		{
			return arg1 < arg2;
		}

		template <typename T>
		static typename std::enable_if<!types::lesserExists<T>::value, bool>::type
		lesser(const T& arg1, const T& arg2) noexcept
		{
			return std::memcmp(&arg1, &arg2, sizeof(T)) < 0;
		}
	}
}
