#pragma once

#include "string.h"
#include "array.h"

namespace z
{
	namespace core
	{
		/**
		 * \brief Split a string into an array of strings according to a specified delimiter.
		 *
		 * \param input The string to split into an array.
		 * \param delim The delimiter that separates items.
		 *
		 * \return A null array if the input string has 0 characters.
		 * Otherwise, an array containing all substrings between any occurring delimiters.
		 *
		 * \threadsafe_function_yes
		 */
		template <encoding E>
		array< string<E> > split(const string<E>& input, const string<E>& delim)
		{
			array< string<E> > list;

			int beg = 0;
			int end = input.findAfter(delim,beg);
			while (end >= 0)
			{
				list.add(input.substr(beg,end-beg));

				beg = end + delim.length();
				end = input.findAfter(delim,beg);
			}

			list.add(input.substr(beg, input.length()-beg));

			return list;
		}
	}
}
