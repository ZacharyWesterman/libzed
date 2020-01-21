#pragma once

#include <sys/stat.h>
#include <z/core/string.hpp>

#ifdef _WIN32
#elif __linux__
#define _stat stat
#else
#error file::info incompatible with target OS.
#endif

namespace z
{
	namespace file
	{
		/**
		 * \brief A class to allow a platform-independent
		 * way to get information on a file.
		 *
		 * As not all Linux file system options have a
		 * Windows analog, this class is not a full wrapper
		 * for the \b stat struct. It does however allow for
		 * retrieval of the most common file information
		 * (e.g. file size, modification date, etc).
		 */
		class info
		{
		private:
			struct _stat fileStat;
			bool does_exist;

		public:
			/**
			 * \brief Constructor with file path.
			 *
			 * \param path a string of the path to
			 * the file object.
			 */
			info(const zpath& path);

			/**
			 * \brief Get whether the file object exists
			 * and can be accessed.
			 *
			 * \return \b False if the file does not exist
			 * or cannot be accessed for some reason.
			 * \b True otherwise.
			 */
			inline bool exists();

			/**
			 * \brief Get the time that the file was last accessed.
			 *
			 * \return When the file was last accessed, if it exists.
			 * \b 0 otherwise.
			 */
			time_t lastAccessed();

			/**
			 * \brief Get the time that the file was last modified.
			 *
			 * \return When the file was last modified, if it exists.
			 * \b 0 otherwise.
			 */
			time_t lastModified();

			/**
			 * \brief Get the time that the file's status was last changed.
			 *
			 * \return When the file status was last changed, if it exists.
			 * \b 0 otherwise.
			 */
			time_t lastStatusChange();

			/**
			 * \brief Get the size of the file.
			 *
			 * \return The size of the file (in Bytes), if it exists.
			 * \b 0 otherwise.
			 */
			size_t byteCount();

			/**
			 * \brief Get the number of the device where the file is stored.
			 *
			 * \return The device number of the file, if it exists.
			 * \b 0 otherwise.
			 */
			unsigned int deviceNumber();
		};
	}
}
