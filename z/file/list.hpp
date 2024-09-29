#pragma once

#include "../core/generator.hpp"
#include "../core/string.hpp"

#ifdef __linux__
#include <dirent.h>
#elif _WIN32
#include <windows.h>
#else
#error file::list is incompatible with target OS.
#endif

namespace z {
namespace file {

/**
 * @brief State management struct for directory scanning generators.
 */
struct dirscan {
#ifdef __linux__
	/// Directory handle.
	DIR *dpdf;
	/// File handle.
	dirent *epdf;
	/// The file type to filter by.
	zpath fileType;
#elif _WIN32
	/// Directory handle.
	WIN32_FIND_DATA fd;
	/// File handle.
	HANDLE hFind;
#endif
	/// Whether to include hidden files in the output.
	bool showAll;
	///(reserved) Whether the directory has been opened yet.
	bool used;

	/// Destructor.
	~dirscan();
};

/**
 * @brief List all files of a given type
 * in the given directory.
 *
 * This function is meant to be a
 * platform-independent way of allowing the user
 * to get a list of all files in the given
 * directory with the given file extension.
 *
 * @param dir the working directory. If \b dir is \b "",
 * then it is assumed to be the current working directory.
 *
 * @param fileType the file extension. If the type is \b "*",
 * then all types are accepted. Otherwise, the file type
 * is expected to have no leading period.
 *
 * @param showAll flag specifying whether to include
 * hidden files in the output.
 *
 * @return A generator that will list all files of the given type in the given directory.
 */
core::generator<zpath, dirscan> listFiles(const zpath &dir, const zpath &fileType = "*", bool showAll = true) noexcept;

/**
 * @brief List all sub-directories
 * in the given directory.
 *
 * This function is meant to be a
 * platform-independent way of allowing the user
 * to get a list of all sub-directories in the given
 * directory with the given file extension.
 *
 * @param dir the working directory. If \b dir is \b "",
 * then it is assumed to be the current working directory.
 *
 * @param showAll flag specifying whether to include
 * hidden directories in the output.
 *
 * @return A generator that will list all directories in the given directory.
 */
core::generator<zpath, dirscan> listDirs(const zpath &dir, bool showAll = false) noexcept;
} // namespace file
} // namespace z
