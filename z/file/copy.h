#pragma once

#include <z/core/string.h>


namespace z
{
	namespace file
	{
		/**
		 * \brief Copy data from one file to another.
		 *
		 * \param fileInput the path of the file to copy from.
		 * \param fileOutput the path of the file to copy to.
		 *
		 * \return \b True if the file was copied successfully.
		 * \b False otherwise.
		 */
		bool copy(const core::string<utf8>& fileInput, const core::string<utf8>& fileOutput);
	}
}