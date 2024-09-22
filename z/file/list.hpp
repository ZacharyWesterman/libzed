#pragma once

#include "../core/array.hpp"
#include "../core/string.hpp"

namespace z {
namespace file {
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
 * @param file_type the file extension. If the type is \b "*",
 * then all types are accepted. Otherwise, the file type
 * is expected to have no leading period.
 *
 * @return An array containing the names of all files of
 * the given type in the given directory.
 */
core::array<zpath> listFiles(const zpath &dir, const zpath &file_type) noexcept;

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
 * @return An array containing the names of all
 * sub-directories in the given directory.
 */
core::array<zpath> listDirs(const zpath &dir, bool showAll = false) noexcept;
} // namespace file
} // namespace z
