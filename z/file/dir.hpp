#pragma once

#include <z/core/string.h>

namespace z
{
	namespace file
	{
		/**
		 * \brief Get the current working directory.
		 * \return A string containing the current directory the executable is running from.
		 * \threadsafe_function_yes
		 */
		zpath dir();
	}
}
