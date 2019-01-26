#pragma once

#include "string.h"
#include "array.h"

namespace z
{
	namespace core
	{
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
