#pragma once

#include "../core/sizable.hpp"
#include "../core/string.hpp"
#include <sys/stat.h>

#ifdef _WIN32
#elif __linux__
#define _stat stat
#else
#error file::info is incompatible with this OS! Please create a pull request or open an issue on GitHub.
#endif

namespace z {
namespace file {
/**
 * @brief A class to allow a platform-independent
 * way to get information on a file.
 *
 * As not all Linux file system options have a
 * Windows analog, this class is not a full wrapper
 * for the \b stat struct. It does however allow for
 * retrieval of the most common file information
 * (e.g. file size, modification date, etc).
 */
class info : core::sizable {
private:
	struct _stat fileStat;
	bool does_exist;

public:
	/**
	 * @brief Constructor with file path.
	 *
	 * @param path a string of the path to
	 * the file object.
	 */
	info(const zpath &path) noexcept;

	/**
	 * @brief Get whether the file object exists
	 * and can be accessed.
	 *
	 * @return \b False if the file does not exist
	 * or cannot be accessed for some reason.
	 * @b True otherwise.
	 */
	bool exists() const noexcept;

	/**
	 * @brief Get the time that the file was last accessed.
	 *
	 * @return When the file was last accessed, if it exists.
	 * @b 0 otherwise.
	 */
	time_t accessed() const noexcept;

	/**
	 * @brief Get the time that the file was last modified.
	 *
	 * @return When the file was last modified, if it exists.
	 * @b 0 otherwise.
	 */
	time_t modified() const noexcept;

	/**
	 * @brief Get the time that the file's status was last changed.
	 *
	 * @return When the file status was last changed, if it exists.
	 * @b 0 otherwise.
	 */
	time_t changed() const noexcept;

	/**
	 * @brief Get the size of the file.
	 *
	 * @return The size of the file (in Bytes), if it exists.
	 * @b 0 otherwise.
	 */
	size_t size() const noexcept override;

	/**
	 * @brief Get the number of the device where the file is stored.
	 *
	 * @return The device number of the file, if it exists.
	 * @b 0 otherwise.
	 */
	int device() const noexcept;

	/**
	 * @brief Get the mode (permissions) of the file.
	 * @return The file permissions.
	 */
	mode_t mode() const noexcept;

	/**
	 * @brief Check if this file is a directory.
	 * @return True if this is a directory, false otherwise.
	 */
	bool directory() const noexcept;

	/**
	 * @brief Check if this file is a symbolic link.
	 * @return True if this is a symlink, false otherwise.
	 */
	bool symlink() const noexcept;

	/**
	 * @brief Check if this file is a regular file.
	 * @return True if this is a file, false otherwise.
	 */
	bool regular() const noexcept;
};
} // namespace file
} // namespace z
