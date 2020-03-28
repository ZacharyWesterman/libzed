#pragma once

/**
 * \file z/core/zstr.hpp
 * \namespace z::core
 */

namespace z
{
	namespace core
	{
		namespace zstr
		{
			/**
			 * \brief Data types that a string can convert to or from.
			 */
			enum zstr
			{
				string   = 0,
				integer  = 1,
				floating = 2,
				complex  = 4
			};
		}
	}
}
