#pragma once

#include <z/core/string.hpp>

namespace z
{
	namespace file
	{
		/**
		 * \brief Change the current working directory.
		 * \param path A string containing the new working directory.
		 * \return true on success, false on failure.
		 * \threadsafe_function_yes
		 */
		bool chdir(const zpath& path) noexcept;
	}
}
