#pragma once

#include "string.h"
#include "array.h"

#include <z/system/console.h>

namespace z
{
	namespace core
	{
		template <encoding E>
		array< string<E> > split(const string<E>& input, const string<E>& delim)
		{
			system::console con;
			array< string<E> > list;

			int beg = 0;
			int end = input.findAfter(delim,beg);
			while (end >= 0)
			{
				string<E> len = end-beg;
				string<E> b = beg;
				string<E> item = input.substr(beg,end-beg);
				(item+" "+beg+" "+len).writeln(con);
				list.add(item);

				beg = end + delim.length();
				end = input.findAfter(delim,beg);
			}

			list.add(input.substr(beg, input.length()-beg+1));

			return list;
		}
	}
}
