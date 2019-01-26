#pragma once

#include "string.h"
#include "array.h"

namespace z
{
	namespace core
	{
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
