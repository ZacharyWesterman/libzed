#pragma once

#include "string.h"
#include "array.h"

namespace z
{
	namespace core
	{
		/**
		 * \brief Concatenate elements in an array into a string, separated by a delimiter.
		 *
		 * \param list An array of strings.
		 * \param delim The delimiter to go between items.
		 *
		 * \return A string containing all elements in order, separated by the delimiter.
		 *
		 * \function_threadsafe_yes
		 */
		template <encoding E>
		string<E> join(const array< string<E> >& list, const string<E>& delim)
		{
			string<E> result;

			for (auto& item : list)
			{
				if (result.length()) result += delim;
				result += item;
			}

			return result;
		}
	}
}
