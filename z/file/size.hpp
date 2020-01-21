#include <z/core/string.hpp>

namespace z
{
	namespace file
	{
		/**
		 * \brief Get the size of a file in bytes
		 *
		 * \param path The file name.
		 * \return The length of the file in bytes, or 0 if the file does not exist.
		 */
		size_t size(const zpath& path);
	}
}
