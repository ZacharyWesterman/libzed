#pragma once

#include "string.hpp"
#include "refArray.hpp"

namespace z
{
	namespace core
	{
		/**
		* \brief Concatenate elements in an array into a string, separated by a delimiter.
		*
		* \param list An array whose elements can implicitly convert to strings.
		* \param delim The delimiter to go between items.
		*
		* \return A string containing all elements in order, separated by the delimiter.
		*
		* \threadsafe_function_yes
		*/
		template <typename T, encoding E>
		string<E> join(const array<T>& list, const string<E>& delim) noexcept
		{
			string<E> result;

			for (auto& item : list)
			{
				if (result.length()) result += delim;
				result += item;
			}

			return result;
		}

		/**
		* \brief Concatenate elements in an array into a string, separated by a delimiter.
		*
		* \param list An array whose elements can implicitly convert to strings.
		* \param delim The delimiter to go between items.
		*
		* \return A string containing all elements in order, separated by the delimiter.
		*
		* \threadsafe_function_yes
		*/
		template <typename T>
		zstring join(const array<T>& list, const zstring& delim) noexcept
		{
			zstring result;

			for (auto& item : list)
			{
				if (result.length()) result += delim;
				result += item;
			}

			return result;
		}

		/**
		* \brief Dereference elements in an array and concatenate them into a string, separated by a delimiter.
		*
		* \param list An array of pointers whose objects can implicitly convert to strings.
		* \param delim The delimiter to go between items.
		*
		* \return A string containing all elements in order, separated by the delimiter.
		*
		* \threadsafe_function_yes
		*/
		template <typename T, encoding E>
		string<E> joinDeref(const array<T>& list, const string<E>& delim) noexcept
		{
			static_assert(std::is_pointer<T>::value, "Array template must be of pointer type.");

			string<E> result;

			for (auto& item : list)
			{
				if (result.length()) result += delim;
				result += *item;
			}

			return result;
		}
	}
}
